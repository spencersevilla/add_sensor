#include "add_hello.h"

int add_generate_hello() {
  struct sk_buff *skb;
  int len = 0;
  struct add_hello_hdr hdr;
  char *data;

  struct add_controller *c;
  struct add_neighbor *n;
  /* we don't yet know how LONG the hello packet will be, 
   * so let's make it first and THEN copy to skb... */
  hdr.family = AF_ADD;
  hdr.pkt_type = ADD_TYPE_HELLO;
  hdr.src_id = add_id;
  memset(hdr.mac, 0, 6);
  hdr.seqno = 0;
  hdr.clist = num_controllers;
  hdr.nlist = num_neighbors;
  len += sizeof(struct add_hello_hdr);

  len += (hdr.clist * sizeof(struct add_clist_entry));
  len += (hdr.nlist * sizeof(struct add_nlist_entry));

  skb = alloc_skb(LL_MAX_HEADER + len, GFP_KERNEL);
  if (unlikely(skb == NULL)) {
    return -ENOBUFS;
  }

  skb_reserve(skb, LL_MAX_HEADER);
    
  /* copy static header information */
  data = skb_put(skb, len);

  memcpy(data, &hdr, sizeof(struct add_hello_hdr));
  data = data + sizeof(struct add_hello_hdr);  

  /* iterate through the controller list... */
  c = controller_list_head;
  while (c != NULL) {
    struct add_clist_entry entry;
    entry.id = c->id;
    entry.hops = c->hops;

    memcpy(data, &entry, sizeof(struct add_clist_entry));
    data = data + sizeof(struct add_clist_entry);
    c = c->next;
  }

  /* iterate through the neighbor list... */
  n = neighbor_list_head;
  while (n != NULL) {
    struct add_nlist_entry entry;
    entry.id = n->id;

    memcpy(data, &entry, sizeof(struct add_nlist_entry));
    data = data + sizeof(struct add_nlist_entry);
    n = n->next;
  }

  /* pass to L2. broadcast_daddr so all devices will hear! */
  return mhost_send_to_l2(skb, dev, broadcast_daddr);
}

int add_receive_hello(struct sk_buff *skb) {
	/* first, parse intro data */
  struct add_neighbor *src = NULL;
  struct add_hello_hdr *hdr = NULL;
  struct add_clist_entry *controller = NULL;
  struct add_controller *c = NULL;

  int i = 0;
  printk(KERN_INFO "add: add_receive_hello called\n");
  hdr = (struct add_hello_hdr *) skb_pull(skb, sizeof(struct add_hello_hdr));

  /* first, update neighbor table to reflect that the
   * node sending this message is a 1-hop neighbor! */
  src = neighbor_from_list(hdr->src_id);
  if (src == NULL) {
    /* we've never seen this neighbor before! */
    src = kmalloc( sizeof(struct add_neighbor), GFP_KERNEL);
    src->id = hdr->src_id;
    src->hops = 1;
    memset(src->daddr, 0, 6);
    insert_neighbor_list(src);
  } else {
    /* now it's one hop away, even if it used to be 2! */
    src->id = 1;
  }

  /* next, go through this node's controller-list
   * and update our own controller-list as necessary. */
  for (i = 0; i < hdr->clist; i++) {
    controller = (struct add_clist_entry *) skb_pull(skb, sizeof(struct add_clist_entry));
    c = controller_from_list(controller->id);
    if (c == NULL) {
      /* this is a new controller we've never seen before!!! */
      c = kmalloc( sizeof(struct add_controller), GFP_KERNEL);
      c->id = controller->id;
      c->next_hop = src;
      c->hops = 2;
      c->seqno = 0;
      insert_controller_list(c);
    } else {
      /* we have a match, so now see if we want to update the path? */
      if ((controller->hops + 1) < c->hops) {
        /* shorter route to the controller through this node */
        c->hops = (controller->hops + 1);
        c->next_hop = src;
      }
    }
  }

  /* last, do the same for the neighbor-list??? huh??? */
  return 0;
}

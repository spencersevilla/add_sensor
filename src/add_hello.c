#include "add_includes.h"
#include "mnet_includes.h"
#include "kernel_includes.h"

int add_generate_hello() {
  struct sk_buff *skb;
  int len = 0;
  struct add_hello_hdr hdr;
  char *data;

  struct add_controller *c;
  struct add_neighbor *n;

  /* we don't yet know how LONG the hello packet will be, 
   * so let's make it first and THEN copy it to skb... */
  hdr.family = AF_ADD;
  hdr.pkt_type = ADD_TYPE_HELLO;
  hdr.src_id = add_id;
  hdr.is_controller = is_controller;
  hdr.seqno = 0;
  memcpy(hdr.mac, my_mac, 6);
  hdr.clist = num_controllers;
  hdr.nlist = num_1hop_neighbors();
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
    /* only send direct (1-hop) neighbors */
    if (n->hops == 1) {
      struct add_nlist_entry entry;
      entry.id = n->id;

      memcpy(data, &entry, sizeof(struct add_nlist_entry));
      data = data + sizeof(struct add_nlist_entry);
    }

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
  struct add_nlist_entry *nentry = NULL;
  struct add_neighbor *neighbor = NULL;

  int i = 0;
  // printk(KERN_INFO "add: add_receive_hello called\n");

  hdr = (struct add_hello_hdr *) skb_network_header(skb);

  /* first, update neighbor table to reflect that the
   * node sending this message is a 1-hop neighbor! */
  src = neighbor_from_list(hdr->src_id);
  if (src == NULL) {
    printk(KERN_INFO "add: new neighbor %d found!", hdr->src_id);
    /* we've never seen this neighbor before! */
    src = kmalloc( sizeof(struct add_neighbor), GFP_KERNEL);
    src->id = hdr->src_id;
    src->hops = 1;
    memcpy(src->daddr, hdr->mac, 6);
    src->seen = 1;
    insert_neighbor_list(src);
  } else {
    /* update if this used to be 2-hop neighbor */
    if (src->hops == 2) {
      src->hops = 1;
      memcpy(src->daddr, hdr->mac, 6);
    }
    src->seen = 1;
  }

  /* is it a controller? */
  if (hdr->is_controller) {
    c = controller_from_list(hdr->src_id);
    if (c == NULL) {
      printk(KERN_INFO "add: new controller %d found!", hdr->src_id);
      /* we've never seen this controller before! */
      c = kmalloc(sizeof(struct add_controller), GFP_KERNEL);
      c->id = hdr->src_id;
      c->hops = 1;
      c->next_hop = src;
      c->seqno = 0;
      insert_controller_list(c);
    } else {
      /* mark that controller is one hop away */
      c->hops = 1;
      c->next_hop = src;
    }
  }

  /* next, go through this node's controller-list
   * and update our own controller-list as necessary. */
  for (i = 0; i < hdr->clist; i++) {
    controller = (struct add_clist_entry *) skb_pull(skb, sizeof(struct add_clist_entry));

    if (controller->id == add_id) {
      /* this is just us! do nothing */
      continue;
    }

    c = controller_from_list(controller->id);
    if (c == NULL) {
      printk(KERN_INFO "add: new controller %d found!", controller->id);
      /* this is a new controller we've never seen before!!! */
      c = kmalloc(sizeof(struct add_controller), GFP_KERNEL);
      c->id = controller->id;
      c->next_hop = src;
      c->hops = controller->hops + 1;
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

  /* last, do the same for the neighbor-list */
  for (i = 0; i < hdr->nlist; i++) {
    nentry = (struct add_nlist_entry *) skb_pull(skb, sizeof(struct add_nlist_entry));

    if (nentry->id == add_id) {
      /* this is just us! do nothing */
      continue;
    }

    neighbor = neighbor_from_list(nentry->id);
    if (neighbor == NULL) {
      /* this is a new neighbor we've never seen before!
       * assume 2 hops and update if/when we get HELLO */
      printk(KERN_INFO "add: new neighbor %d found!", nentry->id);
      neighbor = kmalloc(sizeof(struct add_neighbor), GFP_KERNEL);
      neighbor->id = nentry->id;
      neighbor->hops = 2;
      /* give it the src mac for now. If this becomes a 
       * 1-hop neighbor we'll update when we get HELLO. */
      memcpy(neighbor->daddr, src->daddr, 6);
      insert_neighbor_list(neighbor);
    }
      /* NO ELSE: we already have a match, but it's at LEAST
       * 2-hops close, so there's no benefit. Just ignore! */
  }

  kfree_skb(skb);
  return 0;
}

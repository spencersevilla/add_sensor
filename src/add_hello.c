#include "add_hello.h"

int add_generate_hello() {
   struct sk_buff *skb;

   skb = alloc_skb(MHOST_MAX_HEADER + ADD_HELLO_SIZE, sk->sk_allocation);
   if (unlikely(skb == NULL))
       return -ENOBUFS;
   skb_reserve(skb, MHOST_MAX_HEADER);
    
    /* copy payload from userspace */
//    data = skb_put(skb, len);
	
	return 0;
}

int add_receive_hello(struct sk_buff *skb) {
	/* first, parse intro data */
  struct add_neighbor *src = NULL;
  struct add_hello_hdr *hdr = NULL;
  struct add_clist_entry *controller = NULL;
  struct add_controller *c = NULL;

  int i = 0;
  printk(KERN_INFO "add: add_receive_hello called\n");
  hdr = skb_pull(skb, sizeof(struct add_hello_hdr));

  /* first, update neighbor table to reflect that the
   * node sending this message is a 1-hop neighbor! */
  src = neighbor_from_list(hdr->src_id);
  if (src == NULL) {
    /* we've never seen this neighbor before! */
    src = kmalloc( sizeof(struct add_neighbor), GFP_KERNEL);
    src->id = hdr->src_id;
    src->hops = 1;
    src->daddr = NULL;
    insert_neighbor_list(src);
  } else {
    /* now it's one hop away, even if it used to be 2! */
    src->id = 1;
  }

  /* next, go through this node's controller-list
   * and update our own controller-list as necessary. */
  for (i = 0; i < hdr->clist; i++) {
    controller = skb_pull(skb, sizeof(struct add_clist_entry));
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

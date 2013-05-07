#include "add_includes.h"
#include "mnet_includes.h"

int get_anchor_hash(int req_id);

int add_generate_req(int req_id) {
  struct sk_buff *skb;
  int anchor_controller = 0;
  int len = sizeof(struct add_req_pkt);
  struct add_req_pkt pkt;
  char *data;

  printk(KERN_INFO "add: add_generate_req called\n");

  anchor_controller = get_anchor_hash(req_id);
  if (anchor_controller == -1) {
  	printk(KERN_INFO, "add error: cannot hash req_id!");
  	return -1;
  }

  pkt.family = AF_ADD;
  pkt.pkt_type = ADD_TYPE_REQ;
  pkt.dst_id = anchor_controller;
  pkt.src_id = add_id;
  pkt.src_controller = local_controller_id;
  pkt.req_id = req_id;

  skb = alloc_skb(LL_MAX_HEADER + len, GFP_KERNEL);
  if (unlikely(skb == NULL)) {
    return -ENOBUFS;
  }

  skb_reserve(skb, LL_MAX_HEADER);
  data = skb_put(skb, len);
  memcpy(data, &pkt, sizeof(struct add_req_pkt));

  return route_to_controller(anchor_controller, skb);
}

int add_receive_req(struct sk_buff *skb) {
  struct add_req_pkt *pkt;

  printk(KERN_INFO "add: add_receive_req called\n");
  pkt = (struct add_req_pkt *) skb_network_header(skb);

  if (pkt->dst_id == add_id) {
 	printk(KERN_INFO "add error: sensor chosen as anchor controller???\n");
 	return -1;
  }

  /* just keep on routing it! */
  return route_to_controller(pkt->dst_id, skb);
}

int get_anchor_hash(int req_id) {
	/* hash function here! */
	return 1;
}
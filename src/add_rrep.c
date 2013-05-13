#include "add_includes.h"
#include "mnet_includes.h"

int process_rep(struct sk_buff *skb);

int add_receive_rep(struct sk_buff *skb) {
  struct add_rep_pkt *pkt;

  printk(KERN_INFO "add: add_receive_rep called\n");
  pkt = (struct add_rep_pkt *) skb_network_header(skb);

  /* packet detsined for us! */
  if (pkt->dst_id == add_id) {
 	  return process_rep(skb);
  }

  /* not us, just keep on routing it! */
  return route_to_node(pkt->dst_id, pkt->dst_controller, skb);
}

int process_rep(struct sk_buff *skb) {
  struct add_rep_pkt *pkt;
  struct add_node *node;

  printk(KERN_INFO "add: add_process_rep called\n");
  pkt = (struct add_rep_pkt *) skb_network_header(skb);

  node = (struct add_node *) kmalloc(sizeof(struct add_node), GFP_KERNEL);
  node->id = pkt->req_id;
  node->controller_id = pkt->req_controller;
  insert_node_list(node);

  skb_free(skb);
  return 0;
}
#include "add_includes.h"
#include "mnet_includes.h"
#include "kernel_includes.h"

/* req successfully received! this code only run by a controller. 
 * basically, we do our checks and generate a RREP to send back. 
 * NOTE that we just use the normal node-list, this way we automatically 
 * gain routing access to all the nodes we anchor for! :-) */
int add_process_req(struct add_req_pkt *pkt) {
  struct add_node *node;

  struct sk_buff *skb;
  int len = sizeof(struct add_rep_pkt);
  struct add_rep_pkt *response;
  int answer = 0;

  printk(KERN_INFO "add: add_process_req called\n");

  node = node_from_list(pkt->req_id);
  if (node != NULL) {
    answer = node->controller_id;
  } else {
  	/* we should log that we couldn't find an answer! */
  	printk(KERN_INFO "add: received a RREQ for a node that we don't know.\n");
  }

  skb = alloc_skb(LL_MAX_HEADER + len, GFP_KERNEL);
  if (unlikely(skb == NULL)) {
    return -ENOBUFS;
  }

  skb_reserve(skb, LL_MAX_HEADER);
  response = skb_put(skb, len);

  response->family = AF_ADD;
  repsonse->pkt_type = ADD_TYPE_REP;
  response->dst_id = pkt->src_id;
  response->dst_controller = pkt->src_controller;
  response->src_id = add_id;
  response->req_id = pkt->req_id;
  response->req_controller = answer;

  return route_to_node(response->dst_id, response->dst_controller, skb);
}
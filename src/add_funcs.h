#ifndef _add_funcs_h
#define _add_funcs_h

/* this file contains all functions called outside of the file they're in */

#include "kernel_includes.h"
#include "add_defs.h"
#include "add_structs.h"

/* add_hello.c */
int add_generate_hello(void);
int add_receive_hello(struct sk_buff *skb);

/* add_sendmsg.c */
int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len);
int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, struct net_device *orig_dev);

/* add.c */
int add_init(void);
int route_to_node(int dst_id, int dst_controller, struct sk_buff *skb);
int route_to_controller(int cid, struct sk_buff *skb);

void * lookup_next_hop(int dst_id);
int local_controller_for_node(int node_id);
void * next_hop_for_controller(int controller_id);

/* add_tables.c */
int insert_node_list(struct add_node *node);
int remove_node_list(int node_id);
struct add_node * node_from_list(int node_id);
int insert_controller_list(struct add_controller *node);
int remove_controller_list(int controller_id);
struct add_controller * controller_from_list(int controller_id);
int insert_neighbor_list(struct add_neighbor *node);
int remove_neighbor_list(int neighbor_id);
struct add_neighbor * neighbor_from_list(int neighbor_id);

/* add_rreq.c */
int add_generate_req(int req_id);
int add_receive_req(struct sk_buff *skb);

/* add_rrep.c */
int add_receive_rep(struct sk_buff *skb);

#endif
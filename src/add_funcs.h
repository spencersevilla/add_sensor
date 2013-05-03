#ifndef _add_funcs_h
#define _add_funcs_h

/* this file contains all functions called outside of the file they're in */

#include "kernel_includes.h"
#include "add_defs.h"
#include "add_structs.h"

/* add_hello.c */
int add_generate_hello(void);
int add_receive_hello(void);

/* add_sendmsg.c */
int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len);
int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, struct net_device *orig_dev);

/* add.c */
int add_init(void);
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

#endif
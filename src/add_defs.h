#ifndef _add_defs_h
#define _add_defs_h

#include "kernel_includes.h"

extern int is_controller;
extern int add_id;
extern int local_controller_id;

extern int num_controllers;
extern int num_neighbors;
extern int num_nodes;

extern char my_mac[6];
extern char broadcast_daddr[6];

extern struct net_device *dev;
extern struct delayed_work hello_timer_dw;
extern struct add_node *node_list_head;
extern struct add_neighbor *neighbor_list_head;
extern struct add_controller *controller_list_head;

#define HELLO_INTERVAL (3*HZ)
#define CLEANUP_INTERVAL ((2 * HELLO_INTERVAL) + (1*HZ))

#define ADD_HELLO_SIZE 8

#define AF_ADD 1

#define ADD_TYPE_HELLO 1
#define ADD_TYPE_DATA 2
#define ADD_TYPE_REQ 3
#define ADD_TYPE_REP 4
#define ADD_TYPE_CONTROLLER_REQ 5
#define ADD_TYPE_CONTROLLER_REP 6

#endif
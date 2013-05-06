#ifndef _add_defs_h
#define _add_defs_h

#include "kernel_includes.h"

extern int is_controller;
extern int add_id;

extern int num_controllers;
extern int num_neighbors;
extern int num_nodes;

extern char broadcast_daddr[6];

extern struct net_device *dev;
extern struct delayed_work hello_timer_dw;
extern struct add_node *node_list_head;
extern struct add_neighbor *neighbor_list_head;
extern struct add_controller *controller_list_head;

#define HELLO_INTERVAL (3*HZ)
#define CLEANUP_INTERVAL (HELLO_INTERVAL + 1*HZ)

#define ADD_HELLO_SIZE 8

#define AF_ADD 1

#define ADD_TYPE_HELLO 1
#define ADD_TYPE_DATA 2

#endif
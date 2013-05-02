#ifndef _add_h
#define _add_h

#include "kernel_includes.h"

struct add_controller {
	int id;
	char next_hop[6];
	int distance;
	int seqno;
	struct add_controller *next;
};

struct add_node {
	int id;
	int controller_id;
	struct add_node *next;
};

struct addhdr {
    __be16 family; /* MUST BE FIRST FIELD!!! */
    __be16 ones;   /* all ones for test here */
};

extern int is_controller;
extern struct delayed_work hello_timer_dw;

int add_init(void);
void * lookup_next_hop(int dst_id);
int local_controller_for_node(int node_id);
void * next_hop_for_controller(int controller_id);

#define HELLO_INTERVAL (3*HZ)
#define CLEANUP_INTERVAL (HELLO_INTERVAL + 1*HZ)

#endif

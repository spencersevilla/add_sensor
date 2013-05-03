#ifndef _add_structs_h
#define _add_structs_h

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

#endif
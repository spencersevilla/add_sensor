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

struct sockaddr_add {
    short			sa_family;
    unsigned short	port;
    unsigned int	id;
    char			blank[8];
};

struct addhdr {
    __be16 family; /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 dst_id;
    __be32 src_id;
};

#endif
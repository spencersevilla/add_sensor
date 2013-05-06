#ifndef _add_structs_h
#define _add_structs_h

/* structs for use in tables */

struct add_neighbor {
    int id;
    int hops;       /* 1 or 2 */
    char daddr[6];  /* next-hop MAC */
    struct add_neighbor *next;
};

struct add_controller {
	int id;
	struct add_neighbor *next_hop;
	int hops;
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

/* structs for use in packet-headers */

struct add_data_hdr {
    __be16 family;      /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 dst_id;
    __be32 src_id;
};

struct add_hello_hdr {
    __be16 family;      /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 src_id;
    char mac[6];
    __be32 seqno;
    __be16 clist;
    __be16 nlist;
};

struct add_clist_entry {
    __be32 id;
    __be32 hops;
};

struct add_nlist_entry {
    __be32 id;
};

#endif
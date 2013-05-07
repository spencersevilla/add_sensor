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

struct add_hdr_info {
    __be16 family;      /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
};

struct add_data_hdr {
    __be16 family;      /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 dst_id;
    __be32 dst_controller; /* 0 indicates already-sent */
    __be32 src_id;
};

struct add_hello_hdr {
    __be16 family;      /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 src_id;
    __be32 is_controller;
    __be32 seqno;
    char mac[6];
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

struct add_req_pkt {
    __be16 family;          /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 dst_id;          /* anchor controller id */
    __be32 src_id;          /* for rrep */
    __be32 src_controller;  /* for rrep */
    __be32 req_id;          /* the id in question */
};

struct add_rep_pkt {
    __be16 family;          /* MUST BE FIRST FIELD!!! */
    __be16 pkt_type;
    __be32 dst_id;          /* requesting node id */
    __be32 dst_controller;  /* dst node's controller */
    __be32 src_id;
    __be32 req_id;          /* the id in question */
    __be32 req_controller;  /* the id's local controller */
};

#endif
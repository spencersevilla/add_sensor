#ifndef _add_sendmsg_h
#define _add_sendmsg_h

#include "kernel_includes.h"
#include "mnet_includes.h"
#include "add_includes.h"

int process_data_pkt(struct sk_buff *skb);
int route_next_hop(struct add_data_hdr *hdr, struct sk_buff *skb):

#endif

#ifndef _add_sendmsg_h
#define _add_sendmsg_h

#include "kernel_includes.h"
#include "mnet_includes.h"
#include "add_includes.h"

int process_data_pkt(struct addhdr *hdr, struct sk_buff *skb);
int route_packet(struct addhdr *hdr, struct sk_buff *skb);

#endif

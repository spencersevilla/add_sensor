#ifndef _add_sendmsg_h
#define _add_sendmsg_h

#define AF_ADD 1

#include "kernel_includes.h"
#include "mnet_includes.h"
#include "add_includes.h"

int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len);
int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, struct net_device *orig_dev);

#endif

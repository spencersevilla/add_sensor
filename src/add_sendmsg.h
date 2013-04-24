#ifndef _add_sendmsg_h
#define _add_sendmsg_h

#define AF_ADD 1

/* routing-layer specific */
struct addhdr {
    __be16 family; /* MUST BE FIRST FIELD!!! */
    __be16 ones;   /* all ones for test here */
};

int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len);
int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, struct net_device *orig_dev);

#endif

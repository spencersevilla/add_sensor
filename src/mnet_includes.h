#ifndef _MHOST_INCLUDES_H
#define _MHOST_INCLUDES_H

struct sockaddr_mhost {
    short           sa_family;
    unsigned short  port;
    unsigned short  id_no;
    char            opaque[10];
};

struct mhost_proto {
    short family;
    struct module *owner;
    struct mhost_proto *next;
    
    /* IPv4/IPv6 hook-in wrapper */
    int (*udp_sendmsg) (struct kiocb *iocb, struct sock *sk,
                        struct msghdr *msg, size_t len);
    int (*sendmsg) (struct sock *sk, struct sk_buff *skb,
                    struct sockaddr *sa, int len);
    int (*rcv) (struct sk_buff *skb, struct net_device *dev,
                struct net_device *orig_dev);
};

extern int mhost_send_to_l2(struct sk_buff *skb, struct net_device *dev, const void *daddr);
extern int mhost_send_to_l4(struct sk_buff *skb);
extern int mhost_register_proto(struct mhost_proto *proto);

#endif
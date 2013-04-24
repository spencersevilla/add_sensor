int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len)
{
    struct addhdr *hdr;
    struct net_device *dev = NULL;
    char *daddr = NULL;
    struct sockaddr_mhost *sm = (struct sockaddr_mhost *)sa;
    
    printk(KERN_INFO "add_mhost_sendmsg called\n");
    
    /* do routing work to find a device */
    if (sm->sa_family != AF_ADD) {
        printk(KERN_INFO "error: wrong sockaddr type!\n");
        return -1;
    }
    
    /* really intelligent routing! */
    if (sm->id_no == 0) {
        printk(KERN_INFO "id_no == 0\n");
        dev = (sock_net(sk))->loopback_dev;
        daddr = NULL;
    } else {
        dev = dev_get_by_index(sock_net(sk), 3);
        if (!dev) {
            printk(KERN_INFO "error: dev not found!\n");
            dev = (sock_net(sk))->loopback_dev;
        }
        if (sm->id_no == 1) {
            daddr = daddr1;
        } else {
            daddr = daddr2;
        }
    }
    
    /* build header */
    hdr = (struct addhdr *)skb_push(skb, sizeof(struct addhdr));
    hdr->family = AF_ADD;
    hdr->ones = 0xFFFF;
    
    printk(KERN_INFO "sending to: [%p:%s]\n", dev, dev->name);

    /* send down the stack! */
    /* NOTE: dst must be set if you want to use an actual interface! */
    return mhost_finish_output(skb, dev, daddr);
};

int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, 
                   struct net_device *orig_dev)
{
    struct addhdr *hdr;
    
    printk(KERN_INFO "add_mhost_rcv called\n");
    
    /* preserve network_header location */
    skb_pull(skb, sizeof(struct addhdr));
    hdr = (struct addhdr *) skb_network_header(skb);
    
    if (hdr->ones != 0xFFFF) {
        printk(KERN_INFO "error: hdr->ones not all ones!\n");
    }

    mhost_local_deliver(skb);
    return 0;
}
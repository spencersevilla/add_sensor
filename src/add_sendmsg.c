#include "add_sendmsg.h"

int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len)
{
    struct addhdr *hdr;
    struct net_device *dev = NULL;
    char *daddr = NULL;
    int local_id = 0;
    struct sockaddr_add *addr = (struct sockaddr_add *)sa;
    
    printk(KERN_INFO "add_mhost_sendmsg called\n");
    
    /* do routing work to find a device */
    if (addr->sa_family != AF_ADD) {
        printk(KERN_INFO "add error: wrong sockaddr type!\n");
        return -1;
    }
    
    if (dev == NULL) {
        printk(KERN_INFO "add error: no wlan device!\n");
        return -1;
    }

    /* routing logic here */
    local_id = local_controller_for_node(addr->id);
    daddr = lookup_next_hop(addr->id);

    if (daddr == NULL) {
        /* no entry, so ABORT for now... */
        printk(KERN_INFO "add error: no route to id %d", addr->id);
        return -1;
    }

    /* build header */
    hdr = (struct addhdr *)skb_push(skb, sizeof(struct addhdr));
    hdr->family = AF_ADD;
    hdr->pkt_type = ADD_TYPE_DATA;
    hdr->dst_id = 0;
    hdr->src_id = 0;
    
    /* send down the stack! */
    return mhost_send_to_l2(skb, dev, daddr);
};

int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, 
                   struct net_device *orig_dev)
{
    struct addhdr *hdr;
    
    printk(KERN_INFO "add_mhost_rcv called\n");
    
    /* preserve network_header location */
    skb_pull(skb, sizeof(struct addhdr));
    hdr = (struct addhdr *) skb_network_header(skb);
    
    // if (hdr->ones != 0xFFFF) {
    //     printk(KERN_INFO "error: hdr->ones not all ones!\n");
    // }
    if (hdr->pkt_type == ADD_TYPE_DATA) {
        return process_data_pkt(hdr, skb);
    }

    /* we received a packet-type that we don't support... */
    printk(KERN_INFO "add error: invalid pkt_type: %d", hdr->pkt_type);
    return -1;
}

int process_data_pkt(struct addhdr *hdr, struct sk_buff *skb) {
    printk(KERN_INFO "add: process_data_pkt called\n");

    if (hdr->dst_id == add_id) {
        /* we are the final destination, so send to L4! */
        mhost_send_to_l4(skb);
        return 0;
    } else {
        /* we must route it onwards... */
        return route_packet(hdr, skb);
    }
}

int route_packet(struct addhdr *hdr, struct sk_buff skb) {
    char *daddr = NULL;

    daddr = lookup_next_hop(hdr->dst_id);
    if (daddr == NULL) {
        printk(KERN_INFO, "add error: no next hop for dst_id %d", hdr->dst_id);
        return -1;
    }

    /* NOTE that when we push we ALREADY have a pointer here, it's 
     * just hdr and it's already built! Just put it back as-is! */
    skb_push(skb, sizeof(struct addhdr));

    /* send down the stack! */
    return mhost_send_to_l2(skb, dev, daddr);
}

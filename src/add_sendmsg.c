#include "add_sendmsg.h"

int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len)
{
    struct add_data_hdr *hdr;
    struct net_device *dev = NULL;
    int controller_id = 0;
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
    controller_id = local_controller_for_node(addr->id);

    if (controller_id == -1) {
        /* no entry, so ABORT for now... */
        printk(KERN_INFO "add error: no local controller for node id %d", addr->id);
        return -1;
    }

    /* build header */
    hdr = (struct add_data_hdr *)skb_push(skb, sizeof(struct add_data_hdr));
    hdr->family = AF_ADD;
    hdr->pkt_type = ADD_TYPE_DATA;
    hdr->dst_id = addr->id;
    hdr->dst_controller = controller_id;
    hdr->src_id = add_id;
    
    /* send down the stack! */
    return route_to_node(hdr->dst_id, hdr->dst_controller, skb);
};

int add_mhost_rcv(struct sk_buff *skb, struct net_device *dev, 
                   struct net_device *orig_dev)
{
    struct add_hdr_info *hdr;
    
    printk(KERN_INFO "add_mhost_rcv called\n");
    
    /* preserve network_header location. NOTE that we never pull! */
    hdr = (struct add_hdr_info *) skb_network_header(skb);
    
    // if (hdr->ones != 0xFFFF) {
    //     printk(KERN_INFO "error: hdr->ones not all ones!\n");
    // }
    if (hdr->pkt_type == ADD_TYPE_DATA) {
        return process_data_pkt(skb);
    } else if (hdr->pkt_type == ADD_TYPE_HELLO) {
        return add_receive_hello(skb);
    } else if (hdr->pkt_type == ADD_TYPE_REQ) {
        return add_receive_req(skb);
    } else if (hdr->pkt_type == ADD_TYPE_REP) {
        return add_receive_rep(skb);
    }

    /* we received a packet-type that we don't support... */
    printk(KERN_INFO "add error: invalid pkt_type: %d", hdr->pkt_type);
    return -1;
}

int process_data_pkt(struct sk_buff *skb) {
    struct add_data_hdr *hdr = NULL;
    printk(KERN_INFO "add: process_data_pkt called\n");
    /* again, DON'T pull the data just yet... */
    hdr = (struct add_data_hdr *) skb_network_header(skb);

    if (hdr->dst_id == add_id) {
        /* We are the final destination, so send to L4! 
         * Note that NOW we finally skb_pull the header. */
        skb_pull(skb, sizeof(struct add_data_hdr));
        mhost_send_to_l4(skb);
        return 0;
    } else {
        /* we must route it onwards... */
        return route_to_node(hdr->dst_id, hdr->dst_controller, skb);
    }
}
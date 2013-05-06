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
    return route_next_hop(hdr, skb);
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
        return route_next_hop(hdr, skb);
    }
}

/* the header of this func is set and ALREADY pushed! All this
 * func has to do is look up the next-hop daddr and send to l2 */
int route_next_hop(struct add_data_hdr *hdr, struct sk_buff *skb) {
    struct add_neighbor *nxt_hop = NULL;
    struct add_controller *lc = NULL;

    /* STEP 1: check if the dst is within two hops, if so deliver! */
    nxt_hop = neighbor_from_list(hdr->dst_id);

    if (nxt_hop != NULL) {
        return mhost_send_to_l2(skb, dev, nxt_hop->daddr);
    }

    /* STEP 2: forward to local controller's next-hop */
    lc = controller_from_list(hdr->dst_controller);
    if (lc != NULL) {
        nxt_hop = lc->next_hop;
        if (nxt_hop != NULL) {
            return mhost_send_to_l2(skb, dev, nxt_hop->daddr);
        }
    }

    /* STEP 3: no route exists to the local controller, so abort! */
    printk(KERN_INFO "add error: cannot route next-hop for dst id %d and dst controller %d", hdr->dst_id, hdr->dst_controller);
    return -1;
}

// int route_packet(struct add_data_hdr *hdr, struct sk_buff *skb) {
//     char *daddr = NULL;

//     daddr = lookup_next_hop(hdr->dst_id);
//     if (daddr == NULL) {
//         printk(KERN_INFO "add error: no next hop for dst_id %d", hdr->dst_id);
//         return -1;
//     }

//     /* NOTE that when we push we ALREADY have a pointer here, it's 
//      * just hdr and it's already built! Just put it back as-is! */
//     skb_push(skb, sizeof(struct add_data_hdr));

//     /* send down the stack! */
//     return mhost_send_to_l2(skb, dev, daddr);
// }

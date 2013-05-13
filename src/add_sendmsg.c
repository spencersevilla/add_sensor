#include "kernel_includes.h"
#include "mnet_includes.h"
#include "add_includes.h"

int add_mhost_sendmsg(struct sock *sk, struct sk_buff *skb, struct sockaddr *sa, int len)
{
    struct add_data_hdr *hdr;
    struct net_device *dev = NULL;
    int controller_id = 0;
    struct sockaddr_add *addr = (struct sockaddr_add *)sa;
    int add_neighbor *n;
    
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
        /* no local controller, but maybe its a neighbor?... */
        n = neighbor_from_list(addr->id);
        if (n == NULL) {
            /* we have no leads anymore, so abort */
            printk(KERN_INFO "add error: no routable entry!\n");
            return -1;
        }
        /* we're within two hops, indicated by 0 here */
        controller_id = 0;
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

int add_receive_data(struct sk_buff *skb) {
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
    } 

    /* we are the local controller for this node!
     * mark dst_controller = 0 so we don't loop... */
    if (hdr->dst_controller == add_id) {

        if (is_controller == 0) {
            printk(KERN_INFO "add_receive_data error: sent to controller that isn't!");
            return -1;
        }
        hdr->dst_controller = 0;
    }

    /* this isn't us, so we must route it onwards */
    return route_to_node(hdr->dst_id, hdr->dst_controller, skb);
}
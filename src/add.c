#include "add_includes.h"
#include "mnet_includes.h"
#include "kernel_includes.h"

int add_id = -1;
int local_controller_id = -1;
int is_controller = -1;

char my_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char broadcast_daddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct add_controller *controller_list_head;
struct add_node *node_list_head;
struct add_neighbor *neighbor_list_head;
struct net_device *dev;

int add_init(void) {
    /*  look through the devs. we NEED a wlan interface here! Currently I have
     *  no idea how to deal with multiple devs, so the plan is to get one and
     *  just stick with it completely. Not the best, but it'll do for now.
     */
    struct net_device *ndev = NULL;
    dev = NULL;

    read_lock(&dev_base_lock);
    ndev = first_net_device(&init_net);
    while (ndev != NULL) {
        char *n = ndev->name;
    	if (n[0] == 'e') {
            printk(KERN_INFO "found device: %s", ndev->name);
    		dev = ndev;
    		break;
    	}
        ndev = next_net_device(ndev);
    }
    read_unlock(&dev_base_lock);

    if (dev == NULL) {
    	printk(KERN_INFO "add_init error: no suitable wlan device found!");
    	return -1;
    }

    /* copy MAC address over for our own reference :-) */
    memcpy(my_mac, dev->dev_addr, 6);

    if (add_id == -1 || is_controller == -1) {
        printk(KERN_INFO "add_init error: add_id or is_controller not set!");
        return -1;
    }

    /* initialize node_list and controller_list here */
    node_list_head = NULL;
    controller_list_head = NULL;
    neighbor_list_head = NULL;

    return 0;
}

/* the data in skb is already set and pushed! All this
 * func has to do is look up the next-hop daddr and send to l2 */
int route_to_node(int dst_id, int dst_controller, struct sk_buff *skb) {
    struct add_neighbor *nxt_hop = NULL;
    struct add_controller *dc = NULL;

    /* STEP 1: check if the dst is within two hops, if so deliver! */
    nxt_hop = neighbor_from_list(dst_id);

    if (nxt_hop != NULL) {
        return mhost_send_to_l2(skb, dev, nxt_hop->daddr);
    }

    /* STEP 2: forward to local controller's next-hop */
    dc = controller_from_list(dst_controller);
    if (dc != NULL) {
        nxt_hop = dc->next_hop;
        if (nxt_hop != NULL) {
            return mhost_send_to_l2(skb, dev, nxt_hop->daddr);
        }
    }

    /* STEP 3: no route exists to the local controller, so abort! */
    printk(KERN_INFO "add error: cannot route next-hop for dst id %d and dst controller %d", dst_id, dst_controller);
    return -1;
}

/* ditto for this function. */
int route_to_controller(int cid, struct sk_buff *skb) {
  struct add_controller *c = NULL;
  struct add_neighbor *n = NULL;

  c = controller_from_list(cid);
  if (c == NULL) {
    printk(KERN_INFO "add route_to_controller error: no anchor_controller???");
    return -1;
  }

  n = c->next_hop;
  if (n == NULL) {
    printk(KERN_INFO "add route_to_controller error: no controller neighbor???");
    return -1;
  }

  return mhost_send_to_l2(skb, dev, n->daddr);
}

/* this function takes a node_id and returns the MAC address of the
 * next-hop to its local controller id, if we have it! if not, then NULL.
 */
void * lookup_next_hop(int dst_id) {
	int controller_id = local_controller_for_node(dst_id);
	if (controller_id == -1) {
		return NULL;
	}

	return next_hop_for_controller(controller_id);
}

/* this function translates a node_id to its local controller_id or 0 (failure) */
int local_controller_for_node(int node_id) {
	struct add_node *node = node_from_list(node_id);
    if (node == NULL) {
        /* failure */
        return -1;
    }

    return node->controller_id;
}

/* this function translates a controller_id to its next-hop MAC or NULL */
void * next_hop_for_controller(int controller_id) {
	struct add_controller *controller = controller_from_list(controller_id);
    if (controller == NULL) {
        /* failure */
        return NULL;
    }

	return controller->next_hop;
}
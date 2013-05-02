#include "add_includes.h"
#include "kernel_includes.h"

int is_controller = 0;
struct add_controller *controller_list_head;
struct add_node *node_list_head;
struct net_device *dev;

int add_init(void) {
    /*  look through the devs. we NEED a wlan interface here! Currently I have
     *  no idea how to deal with multiple devs, so the plan is to get one and
     *  just stick with it completely. Not the best, but it'll do for now.
     */
    dev = NULL;

    read_lock(&dev_base_lock);
    struct net_device *ndev = first_net_device(&init_net);
    while (ndev) {
    	if (ndev->name[0] == 'w') {
    		dev = ndev;
    		break;
    	}
        ndev = next_net_device(dev);
    }
    read_unlock(&dev_base_lock);

    if (dev == NULL) {
    	printk(KERN_INFO "add_init error: no suitable wlan device found!");
    	return -1;
    }

    /* initialize node_list and controller_list here */
    node_list_head = NULL;
    controller_list_head = NULL;

    if (is_controller == 1) {
        schedule_delayed_work(&hello_timer_dw, HELLO_INTERVAL);
    }

    return 0;
}

/* this function takes a node_id and returns the MAC address of the
 * next-hop to its local controller id, if we have it! if not, then NULL.
 */
void * lookup_next_hop(int dst_id) {
	int controller_id = local_controller_for_node(dst_id);
	if (controller_id == 0) {
		return NULL;
	}

	return next_hop_for_controller(controller_id);
}

/* this function translates a node_id to its local controller_id or 0 (failure) */
int local_controller_for_node(int node_id) {
	struct add_node *ptr = node_list_head;

	while (ptr != NULL) {
		if (ptr->id == node_id) {
			return ptr->controller_id;
		}
		ptr = ptr->next;
	}

	/* failure */
	return 0;
}

/* this function translates a controller_id to its next-hop MAC or NULL */
void * next_hop_for_controller(int controller_id) {
	struct add_controller *ptr = controller_list_head;

	while (ptr != NULL) {
		if (ptr->id == controller_id) {
			return ptr->next_hop;
		}
		ptr = ptr->next;
	}

	/* failure */
	return NULL;
}


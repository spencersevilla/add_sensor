#include "add.h"

int is_controller = 0;
struct add_controller *controller_list_head;
struct add_node *node_list_head;
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

int insert_node_list(struct add_node *node) {
    /* initialize list */
    struct add_node *ptr, *nxt;
    if (node_list_head == NULL) {
        node_list_head = node;
        node->next = NULL;
        return 0;
    }

    /* insert at front of list */
    if (node->id < node_list_head->id) {
        node->next = node_list_head;
        node_list_head = node;
        return 0;
    }

    /* now we can start iterating because we know that
     * node->id is greater than ptr->id */
    ptr = node_list_head;
    nxt = node_list_head->next;
    while(nxt != NULL) {
        if (node->id < nxt->id) {
            node->next = nxt;
            ptr->next = node;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* IF we get here, it means that nxt == NULL and we
     * never inserted node, so do it at the end of the list! */
    ptr->next = node;
    node->next = NULL;
    return 0;
}

int remove_node_list(int node_id) {
    struct add_node *ptr, *nxt;

    /* list is empty, cannot remove anything from it */
    if (node_list_head == NULL) {
        return -1;
    }

    /* node is head of list */
    if (node_list_head->id == node_id) {
        node_list_head = node_list_head->next;
        return 0;
    }

    ptr = node_list_head;
    nxt = node_list_head->next;
    while (nxt != NULL) {
        if (nxt->id == node_id) {
            ptr->next = nxt->next;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* got all the way to the end of the list without a match */
    return -1;
}

int insert_controller_list(struct add_controller *node) {
    /* initialize list */
    struct add_controller *ptr, *nxt;
    if (controller_list_head == NULL) {
        controller_list_head = node;
        node->next = NULL;
        return 0;
    }

    /* insert at front of list */
    if (node->id < controller_list_head->id) {
        node->next = controller_list_head;
        controller_list_head = node;
        return 0;
    }

    /* now we can start iterating because we know that
     * node->id is greater than ptr->id */
    ptr = controller_list_head;
    nxt = controller_list_head->next;
    while(nxt != NULL) {
        if (node->id < nxt->id) {
            node->next = nxt;
            ptr->next = node;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* IF we get here, it means that nxt == NULL and we
     * never inserted node, so do it at the end of the list! */
    ptr->next = node;
    node->next = NULL;
    return 0;
}

int remove_controller_list(int node_id) {
    struct add_controller *ptr, *nxt;

    /* list is empty, cannot remove anything from it */
    if (controller_list_head == NULL) {
        return -1;
    }

    /* node is head of list */
    if (controller_list_head->id == node_id) {
        controller_list_head = controller_list_head->next;
        return 0;
    }

    ptr = controller_list_head;
    nxt = controller_list_head->next;
    while (nxt != NULL) {
        if (nxt->id == node_id) {
            ptr->next = nxt->next;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* got all the way to the end of the list without a match */
    return -1;
}
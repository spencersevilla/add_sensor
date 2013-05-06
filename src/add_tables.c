#include "add_includes.h"

int num_controllers = 0;
int num_neighbors = 0;
int num_nodes = 0;

int insert_node_list(struct add_node *node) {
    /* initialize list */
    num_nodes++;

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
        num_nodes--;
        return 0;
    }

    ptr = node_list_head;
    nxt = node_list_head->next;
    while (nxt != NULL) {
        if (nxt->id == node_id) {
            ptr->next = nxt->next;
            num_nodes--;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* got all the way to the end of the list without a match */
    return -1;
}

struct add_node * node_from_list(int node_id) {
	struct add_node *ptr;
	/* list is empty; cannot contain node */
	if (node_list_head == NULL) {
		return NULL;
	}

	/* node ID is too low, not in the list */
	if (node_list_head->id > node_id) {
		return NULL;
	}

	ptr = node_list_head;
	while (ptr != NULL) {
		if (ptr->id == node_id) {
			return ptr;
		}

		/* we've gone past it, so it can't exist */
		if (ptr->id < node_id) {
			return NULL;
		}

		ptr = ptr->next;
	}

	/* went through the whole list, couldn't find it. */
	return NULL;
}

int insert_controller_list(struct add_controller *node) {
    /* initialize list */
    num_controllers++;

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
        num_controllers--;
        return 0;
    }

    ptr = controller_list_head;
    nxt = controller_list_head->next;
    while (nxt != NULL) {
        if (nxt->id == node_id) {
            ptr->next = nxt->next;
            num_controllers--;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* got all the way to the end of the list without a match */
    return -1;
}

struct add_controller * controller_from_list(int controller_id) {
	struct add_controller *ptr;
	/* list is empty; cannot contain node */
	if (controller_list_head == NULL) {
		return NULL;
	}

	/* node ID is too low, not in the list */
	if (controller_list_head->id > controller_id) {
		return NULL;
	}

	ptr = controller_list_head;
	while (ptr != NULL) {
		if (ptr->id == controller_id) {
			return ptr;
		}

		/* we've gone past it, so it can't exist */
		if (ptr->id < controller_id) {
			return NULL;
		}

		ptr = ptr->next;
	}

	/* went through the whole list, couldn't find it. */
	return NULL;
}

int insert_neighbor_list(struct add_neighbor *node) {
    /* initialize list */
    num_neighbors++;
    struct add_neighbor *ptr, *nxt;
    if (neighbor_list_head == NULL) {
        neighbor_list_head = node;
        node->next = NULL;
        return 0;
    }

    /* insert at front of list */
    if (node->id < neighbor_list_head->id) {
        node->next = neighbor_list_head;
        neighbor_list_head = node;
        return 0;
    }

    /* now we can start iterating because we know that
     * node->id is greater than ptr->id */
    ptr = neighbor_list_head;
    nxt = neighbor_list_head->next;
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

int remove_neighbor_list(int node_id) {
    struct add_neighbor *ptr, *nxt;

    /* list is empty, cannot remove anything from it */
    if (neighbor_list_head == NULL) {
        return -1;
    }

    /* node is head of list */
    if (neighbor_list_head->id == node_id) {
        neighbor_list_head = neighbor_list_head->next;
        num_neighbors--;
        return 0;
    }

    ptr = neighbor_list_head;
    nxt = neighbor_list_head->next;
    while (nxt != NULL) {
        if (nxt->id == node_id) {
            ptr->next = nxt->next;
            num_neighbors--;
            return 0;
        }

        ptr = nxt;
        nxt = ptr->next;
    }

    /* got all the way to the end of the list without a match */
    return -1;
}

struct add_neighbor * neighbor_from_list(int node_id) {
    struct add_neighbor *ptr;
    /* list is empty; cannot contain node */
    if (neighbor_list_head == NULL) {
        return NULL;
    }

    /* node ID is too low, not in the list */
    if (neighbor_list_head->id > node_id) {
        return NULL;
    }

    ptr = neighbor_list_head;
    while (ptr != NULL) {
        if (ptr->id == node_id) {
            return ptr;
        }

        /* we've gone past it, so it can't exist */
        if (ptr->id < node_id) {
            return NULL;
        }

        ptr = ptr->next;
    }

    /* went through the whole list, couldn't find it. */
    return NULL;
}
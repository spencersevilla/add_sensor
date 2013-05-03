#include "add_hello.h"

int add_generate_hello() {
//    struct sk_buff *skb;

//    skb = alloc_skb(MHOST_MAX_HEADER + ADD_HELLO_SIZE, sk->sk_allocation);
//    if (unlikely(skb == NULL))
//        return -ENOBUFS;
//    skb_reserve(skb, MHOST_MAX_HEADER);
    
    /* copy payload from userspace */
//    data = skb_put(skb, len);
	
	return 0;
}

int add_receive_hello() {
	/* first, parse intro data */

	/* next, read the DV and figure out if we need to update anything */

	/* last, if we DID update our routing table, broadcast a hello! */
	return 0;
}

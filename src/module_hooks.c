#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

#include "add_includes.h"

int add_init(void);

int closing = 0;

static void hello_timer(void *irrelevant);

static struct delayed_work hello_timer_dw;
static DECLARE_DELAYED_WORK(hello_timer_dw, hello_timer);

// static struct delayed_work cleanup_module_dw;
// static DECLARE_DELAYED_WORK(cleanup_module_dw, add_cleanup);

#define HELLO_INTERVAL (3*HZ)
#define CLEANUP_INTERVAL (HELLO_INTERVAL + 1*HZ)

/*******************************************************************************/
int init_module(void) {
    printk(KERN_INFO "adding add kernel module");

    add_init();
    return 0;
}

static void hello_timer(void *irrelevant) {
    printk(KERN_INFO "hello_timer fired!");

    add_generate_hello();

    if (closing == 0) {
		schedule_delayed_work(&hello_timer_dw, HELLO_INTERVAL);
    }
}


void cleanup_module(void) {
    printk(KERN_INFO "removing add module");
    closing = 1;
    cancel_delayed_work(&hello_timer_dw);
    // schedule_delayed_work(&cleanup_module_dw, CLEANUP_INTERVAL);
}

int add_init() {
    schedule_delayed_work(&hello_timer_dw, HELLO_INTERVAL);
    return 0;
}

// int add_cleanup() {

// }

MODULE_LICENSE("GPL");

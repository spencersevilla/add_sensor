#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

#include "add_includes.h"

// static struct delayed_work cleanup_module_dw;
// static DECLARE_DELAYED_WORK(cleanup_module_dw, add_cleanup);

static void hello_timer(void *irrelevant);
struct delayed_work hello_timer_dw;
DECLARE_DELAYED_WORK(hello_timer_dw, hello_timer);
static int closing = 0;

module_param(add_id, int, 0000);
MODULE_PARM_DESC(add_id, "ADD unique ID");

module_param(is_controller, int, 0000);
MODULE_PARM_DESC(is_controller, "ADD boolean, self-explanatory");

/*******************************************************************************/
int init_module(void) {
    printk(KERN_INFO "adding add kernel module");
    closing = 0;
    schedule_delayed_work(&hello_timer_dw, HELLO_INTERVAL);
    return add_init();
}

static void hello_timer(void *irrelevant) {
    // printk(KERN_INFO "hello_timer fired!");

    add_generate_hello();

    if (closing != 1) {
		schedule_delayed_work(&hello_timer_dw, HELLO_INTERVAL);
    }
}

void cleanup_module(void) {
    printk(KERN_INFO "removing add module");
    closing = 1;
    cancel_delayed_work(&hello_timer_dw);
    // schedule_delayed_work(&cleanup_module_dw, CLEANUP_INTERVAL);
}


// int add_cleanup() {

// }

MODULE_LICENSE("GPL");

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/sched.h> 

int closing = 0;
static void intrpt_routine(void *irrelevant);

static struct delayed_work task;
static DECLARE_DELAYED_WORK(task, intrpt_routine);

/*******************************************************************************/
int init_module(void) {
    printk(KERN_INFO "adding add module 1");
    printk(KERN_INFO "adding add module 2");
    schedule_delayed_work(&task, 5*HZ);
    return 0;
}

static void intrpt_routine(void *irrelevant) {
    printk(KERN_INFO "INTERRUPT!!!");

    if (closing == 0) {
		schedule_delayed_work(&task, 5*HZ);
    }
}


void cleanup_module(void) {
    printk(KERN_INFO "removing add module 1");
    printk(KERN_INFO "removing add module 2");
    closing = 1;
    cancel_delayed_work(&task);
}

MODULE_LICENSE("GPL");

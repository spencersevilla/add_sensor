#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/sched.h> 

int timerCount = 0;

static struct work_struct task;
static DECLARE_WORK(task, intrpt_routine, NULL);

/*******************************************************************************/
int init_module(void) {
    printk(KERN_INFO "adding add module!");
    schedule_delayed_work(&task, 5*HZ);
    return 0;
}

static void intrpt_routine(void *irrelevant) {
    printk(KERN_INFO "INTERRUPT!!!");
	schedule_delayed_work(&task, 5*HZ);
}


void cleanup_module(void) {
    printk(KERN_INFO "removing add module!");
}

MODULE_LICENSE("GPL");

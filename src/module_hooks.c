int timerCount = 0;

/*******************************************************************************/
int init_module(void)
{
    printk(KERN_INFO "adding add module!");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "removing add module!");
}

MODULE_LICENSE("GPL");

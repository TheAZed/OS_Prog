#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

MODULE_LICENSE("OS");
MODULE_AUTHOR("TheAZed");
MODULE_DESCRIPTION("A module which writes \"Hello OS\"");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello OS\n");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Exiting Hello OS\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
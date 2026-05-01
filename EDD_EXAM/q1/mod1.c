
// This is the format string kernel module define

#define pr_fmt(fmt)      KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>

// Init function
static int __init my_mod_init(void)
{
        pr_info("Mod1 loded successfully\n");
        pr_info("Hello world from mod1!\n");
        return 0;
}

// Exit function
static void __exit my_mod_exit(void)
{
        pr_info("Mod unloded successfully\n");
        pr_info("Goodbye world from mod1!\n");
        return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Hello world module!");


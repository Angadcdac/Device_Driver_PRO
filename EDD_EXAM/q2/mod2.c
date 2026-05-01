
//This is the format string kernel module define
#define pr_fmt(fmt)     KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#define MY_KBUFFER_SIZ  2048
char *kbuffer;

static int __init my_mod_init(void)
{
        pr_info("Mod2 is successfully loaded\n");
        //kmalloc is returning void *
        kbuffer = (char *)kmalloc(MY_KBUFFER_SIZ, GFP_KERNEL);
        if (!kbuffer)
        {
                pr_err("Error in allocated kernel memory!\n");
                return -1;
        }
        pr_info("It has been successfully address of allocated block = %p\n", kbuffer);
return 0;
}

static void __exit my_mod_exit(void)
{
        pr_info("Goodbye world from mod2!\n");
        kfree(kbuffer);
        return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pradeep");
MODULE_DESCRIPTION("Creating a single block memory!");


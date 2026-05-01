#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>  

#include <linux/init.h>  
static int __init my_mod_init(void)
{
        pr_alert("this is the alert message from device driver\n");
	pr_info("Hello world !\n");
	pr_info("Welcome Angad in Device Driver\n");
	pr_info("Ready now for developing the device driver\n");
	pr_info("\033[1;31mThis is RED message\033[0m\n");
        pr_info("\033[1;32mThis is GREEN message\033[0m\n");
        pr_info("\033[1;33mThis is YELLOW message\033[0m\n");
	pr_info("Enjoy this journey with the dedication\n");
	pr_info("Your Github has been started\n");
	return 0;
}

static void __exit my_mod_exit(void)
{
	pr_alert("Goodbye world \n");
	pr_info("End of this program not journey\n");
	return;
}


module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Hello world module!");

/**************#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt**************************/
 //This macro formats all printk/pr_* log messages by automatically
 //prefixing them with the module name (KBUILD_MODNAME).
 //It helps in identifying which kernel module generated the log.
/****************************************************************************************/

/**********#include <linux/module.h>*****************************************************/  
// Required for all kernel modules.
// Provides macros like MODULE_LICENSE, MODULE_AUTHOR,
// and functions/macros for module loading/unloading.
/****************************************************************************************/

/******************include <linux/init.h>************************************************/  
// Contains macros for initialization and cleanup functions,
// such as __init and __exit.
// Helps the kernel optimize memory usage by freeing init code after use.
/****************************************************************************************/

/********************static int __init my_mod_init(void)*********************************/
// Initialization function of the kernel module.
// This function is executed when the module is loaded using insmod.
// The __init macro tells the kernel that this function is only needed
// during initialization, so its memory can be freed after execution.
// It returns 0 on success and a negative value on failure.
/***************************************************************************************/

/**********static void __exit my_mod_exit(void)******************************************/
// It performs cleanup tasks like freeing memory, releasing resources, etc.
// Cleanup function of the kernel module.
// This function is executed when the module is removed using rmmod.
// The __exit macro tells the kernel that this function is only needed
// during module removal.
// It performs cleanup tasks like freeing memory, releasing resources, etc.
/****************************************************************************************/

/*************************module_init(my_mod_init);**************************************/
// Registers the module’s initialization function with the kernel.
// When you run `insmod mod1.ko`, the kernel automatically calls my_mod_init().
// This is the entry point of the module (runs at load time).
/****************************************************************************************/

/*************module_exit(my_mod_exit);**************************************************/
// Registers the module’s cleanup function with the kernel.
// When you run `rmmod mod1`, the kernel automatically calls my_mod_exit().
// This is used to release resources and perform cleanup before unloading.
/***************************************************************************************/

/****************MODULE_LICENSE("GPL");**************************************************/
// Specifies the license type of the kernel module.
// "GPL" indicates it is open-source and compatible with the Linux 
// kernel.
// Without this, the kernel marks the module as "tainted".
/****************************************************************************************/

/*********MODULE_AUTHOR("EDD <edd@cdac.gov.in>");****************************************/
// Provides information about the author of the module.
// Useful for documentation and debugging purposes.
/****************************************************************************************/

/******************MODULE_DESCRIPTION("Hello world module!");****************************/
//     A short description of what the module does.
//     Displayed in tools like modinfo.
/****************************************************************************************/ 


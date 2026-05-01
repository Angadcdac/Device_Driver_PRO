/*
4. Write a kernel module that taints the kernel. Explain what is a taint? How do tainted modules affect kernel behaviour?
*/
#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>


// function prototypes
//Module functions
static int __init mod_init(void);
static void __exit mod_exit(void);

//Difinitions

static int __init mod_init(void)
{
	pr_info("Hello world!\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Goodbye world!!\n");
	return;
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("ThirdParty");//GNU
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Hello world module!");

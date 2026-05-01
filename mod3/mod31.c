/* Major:Minor static allottment */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define MY_MAJOR_NUM	(202)
#define MY_DEV_NAME	"cdac_edd"

dev_t dev = MKDEV(MY_MAJOR_NUM, 0);

static int __init my_mod_init(void)
{
	int ans;

	pr_info("Hello world from mod31!\n");
	ans = register_chrdev_region(dev, 1, MY_DEV_NAME);
	if (ans < 0)
	{
		pr_info("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));
	return 0;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from mod31!\n");
	unregister_chrdev_region(dev, 1);
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("major:minor static allotment module!");
/***************dev_t dev = MKDEV(MY_MAJOR_NUM, 0);***************************************************/
//    Major Number   |   Minor Number   |
//    (12 bits)   |     (20 bits)   |
//    dev = (major << 20) | minor;
//    // MKDEV macro encodes major and minor numbers into a single dev_t value
/******************************************************************************************************/
// ans = register_chrdev_region(dev, 1, MY_DEV_NAME);
// Registers a range of character device numbers with the kernel
// dev        → starting device number (contains major=202, minor=0)
// 1          → number of devices (only one minor number is reserved)
// MY_DEV_NAME→ name of the device (appears in /proc/devices)
// ans        → returns 0 on success, negative value on failure
/******************************************************************************************************/

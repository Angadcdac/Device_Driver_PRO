/* Automatic device node creation */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#define MY_CLASS_NAME	"cdac_cls"
#define MY_DEV_NAME	"cdac_dev"
#define NUM_DEVS	(5)

dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice[NUM_DEVS];

static int __init my_mod_init(void)
{
	int ans,i, major, minor;
	char dev_string[10]={0};

	pr_info("Hello world from mod33!\n");
	ans = alloc_chrdev_region(&dev, 0, NUM_DEVS, MY_DEV_NAME);
	if (ans<0)
	{
		pr_err("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor range of %d devices %d:%d allotted!\n", NUM_DEVS, MAJOR(dev),MINOR(dev));

	dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
	if (IS_ERR(dev_class))
	{
		pr_err("Could not create device class %s\n", MY_CLASS_NAME);
		goto r_class;
	}

	major = MAJOR(dev);
	for (i=0;i<NUM_DEVS;i++)
	{
		minor = i;
		dev = MKDEV(major, minor);
		sprintf(dev_string, "%s%d", MY_DEV_NAME, i);
		cdevice[i] = device_create(dev_class, NULL, dev, NULL, dev_string);
		if (IS_ERR(cdevice[i]))
		{
			pr_err("Could not create device %s\n", dev_string);
			goto r_device;
		}
	}
	pr_info("%d devices %s under class %s created with success\n", 
			i, MY_DEV_NAME, MY_CLASS_NAME);
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

static void __exit my_mod_exit(void)
{
	//Always destory in reverese order expained below
	pr_info("Goodbye world from mod33!\n");
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Device node creation module!");

/*******************dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);******************************/
// Create a device class in /sys/class/ with name MY_CLASS_NAME.
// THIS_MODULE links the class to the current kernel module./
// This class is later used by device_create() to automatically
// create device files in /dev via udev.
/******************************************************************************************************/

/**********************************dev = MKDEV(major, minor);******************************************/
/* Combine major and minor numbers into a single dev_t value.
 * major → identifies the driver
 * minor → identifies a specific device under that driver
 * This dev is used in functions like cdev_add() and device_create().
 */
/*****************************************************************************************************/
/********************sprintf(dev_string, "%s%d", MY_DEV_NAME, i);*************************************/
/*Create sting like this
 * mydev0
 * mydev1
 * mydev2
 */
/*****************************************************************************************************/
/****************cdevice[i] = device_create(dev_class, NULL, dev, NULL, dev_string);******************/
/* Create a device node in /dev using the given class (dev_class).
 * dev        → contains major & minor number (identifies the device)
 * dev_string → name of the device file (e.g., "mydev0")
 * The returned pointer is stored in cdevice[i] (struct device *)
 * This call also creates an entry in /sys/class/<class_name>/
 * and triggers udev to automatically create /dev/<dev_string>.
 */
/******************************************************************************************************/
//r_device:
//       class_destroy(dev_class);
/* Label for error handling (goto target).
 * If device creation fails, control jumps here.
 * class_destroy() removes the class created in /sys/class/
 */

//r_class:
//        unregister_chrdev_region(dev, 1);
/* Another error handling label.
 * Releases the allocated major/minor numbers (dev_t)
 * so they can be reused by the system.
 */

//        return -1;
/* Return error to indicate driver initialization failed */
/******************************************************************************************************/
/* Note:Cleanup should always be in reverse order of creation:
 * initialization == alloc_chardev_region() --> class_create() --> device_create 
 * destroy        == device_create() --> class_destroy() --> unregiser_chrdev_resgion(); 
 * 
 * device_destroy(dev_class, dev);
 * Removes the device node from /dev (e.g., /dev/mydev0)
 * and deletes its entry from /sys/class/<class_name>/
 * 
 * class_destroy(dev_class);
 * Destroys the device class created by class_create()
 * Removes the directory from /sys/class/
 *
 *unregister_chrdev_region(dev, 1);
 *  Releases the allocated major/minor numbers (dev_t)
 *  so they are free for reuse by other drivers
 */
/******************************************************************************************************/

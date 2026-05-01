
//This is the string format kernel module defined

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MY_CLASS	"pradeep_cls"
#define MY_DEV	        "pradeep_dev"

dev_t dev = 0;//It will allocated dynamic major and minor number of device
static struct class *device_class;
static struct device *char_device;
static struct cdev my_cdev;
int k=0,i=0;

// function prototypes
static int __init my_mod_init(void);
static int my_open(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
static int my_release(struct inode *inode, struct file *file);
static void __exit my_mod_exit(void);

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("My_open function is %s called\n", __func__);
	return 0;
}
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
	pr_info("My_read function is %s called\n", __func__);
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("My_write function is %s called\n", __func__);
	return len;
}

static int my_release(struct inode *inode, struct file *file)
{
	pr_info("My_release function is %s called\n", __func__);
	return 0;
}
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};
static int __init my_mod_init(void)
{
	int val;
	pr_info("Hello world from mod3!\n");
	val = alloc_chrdev_region(&dev, 0, 1, MY_DEV);
	if (val<0)
	{
		pr_err("Error in major:minor number allotment!\n");
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));

	// initialize a cdev
	cdev_init(&my_cdev, &fops);

	val = cdev_add(&my_cdev, dev, 1);//add my_cdev to kernal
	if (val<0)
	{
		pr_err("Error in  add cdev to the kernel!\n");
		goto r_cdev;
	}

	device_class = class_create(THIS_MODULE, MY_CLASS);
	if (IS_ERR(device_class))
	{
		pr_err("Error in  create device class %s\n", MY_CLASS);
		goto r_class;
	}
        char_device = device_create(device_class, NULL, dev, NULL, MY_DEV);
	if (IS_ERR(char_device))
	{
		pr_err("Error in create device %s\n", MY_DEV);
		goto r_device;
	}
	pr_info("Device %s under class %s created with successfully\n", 
			MY_DEV, MY_CLASS);
	return 0;
//error ladder
r_device:
	class_destroy(device_class);
r_class:
	cdev_del(&my_cdev);
r_cdev:
	unregister_chrdev_region(dev, 1);

	return -1;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from mod3!\n");
	device_destroy(device_class, dev);
	class_destroy(device_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pradeep");
MODULE_DESCRIPTION("Read and write operation!");





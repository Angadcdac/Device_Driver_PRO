/* File operations on char device */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#define MY_CLASS_NAME	"cdac_cls"
#define MY_DEV_NAME	"cdac_dev"

dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;

// function prototypes
static int __init my_mod_init(void);
static int my_open(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
static int my_release(struct inode *inode, struct file *file);
static void __exit my_mod_exit(void);

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("My open function %s called\n", __func__);
	return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
	pr_info("My read function %s called\n", __func__);
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("My write function %s called\n", __func__);
	return len;
}

static int my_release(struct inode *inode, struct file *file)
{
	pr_info("My release function %s called\n", __func__);
	return 0;
}

// create a fops struct
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

static int __init my_mod_init(void)
{
	int ans;

	pr_info("Hello world from mod34!\n");
	ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
	if (ans<0)
	{
		pr_err("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));

	// initialize a cdev
	cdev_init(&my_cdev, &fops);

	ans = cdev_add(&my_cdev, dev, 1);
	if (ans<0)
	{
		pr_err("Could not add cdev to the kernel!\n");
		goto r_cdev;
	}

	dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
	if (IS_ERR(dev_class))
	{
		pr_err("Could not create device class %s\n", MY_CLASS_NAME);
		goto r_class;
	}

	cdevice = device_create(dev_class, NULL, dev, NULL, MY_DEV_NAME);
	if (IS_ERR(cdevice))
	{
		pr_err("Could not create device %s\n", MY_DEV_NAME);
		goto r_device;
	}
	pr_info("Device %s under class %s created with success\n", 
			MY_DEV_NAME, MY_CLASS_NAME);
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	cdev_del(&my_cdev);
r_cdev:
	unregister_chrdev_region(dev, 1);
	return -1;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from mod34!\n");
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("File operations on char device module!");
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************************/
// static int my_open(struct inode *inode, struct file *file);
/* 
 * my_open - This function is called when a user-space application 
 * opens the device file (e.g., open("/dev/cdac_dev", O_RDWR)).
 *
 * Parameters:
 *  inode : Represents the device file in the filesystem (contains metadata 
 *          like major/minor number).
 *
 *  file  : Represents the opened file instance. It is used by the kernel 
 *          to track this specific open call and is later passed to read, 
 *          write, and ioctl functions.
 *
 * Return:
 *  0  -> success
 *  <0 -> error (e.g., -EBUSY, -EACCES)
 */
/*******************************************************************************************************/
//static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
/*
 * my_write - Handles write requests from user space to kernel space
 *
 * This function is invoked when a user application calls:
 *      write(fd, user_buffer, len);
 *
 * Parameters:
 *  file : Pointer to struct file representing the opened device instance.
 *         Used to access file-specific data (file->private_data).
 *
 *  buf  : Pointer to user-space buffer (data sent from user program).
 *         Marked with __user to indicate it resides in user space.
 *         MUST use copy_from_user() to access it safely.
 *
 *  len  : Number of bytes the user wants to write.
 *
 *  off  : File offset (not usually used in simple character drivers).
 *
 * Return:
 *  Number of bytes successfully written (len on success),
 *  or negative error code (e.g., -EFAULT)
 */
/*******************************************************************************************************/
// static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
/*
 * my_write - Write handler of the character device driver
 *
 * This function is called when a user-space application performs:
 *      write(fd, buffer, len);
 *
 * Parameters:
 *  file : Pointer to struct file
 *         Represents the opened device instance.
 *         Can be used to access driver-specific data using file->private_data.
 *
 *  buf  : Pointer to user-space buffer
 *         Contains data sent from user application.
 *         Marked with __user → must NOT be accessed directly.
 *         Use copy_from_user() to safely copy data into kernel space.
 *
 *  len  : Number of bytes requested to write
 *
 *  off  : File offset pointer
 *         Usually ignored in simple character drivers
 *
 * Return:
 *  On success → number of bytes written (typically len)
 *  On failure → negative error code (e.g., -EFAULT)
 */
/*******************************************************************************************************/
// static int my_release(struct inode *inode, struct file *file);
/*
 * my_release - Called when the device file is closed
 *
 * This function is invoked when a user-space application calls:
 *      close(fd);
 *
 * Parameters:
 *  inode : Pointer to inode structure
 *          Represents the device file in the filesystem
 *          (contains major/minor number information).
 *
 *  file  : Pointer to struct file
 *          Represents the opened instance of the device.
 *          Used to access private data (file->private_data).
 *
 * Return:
 *  0  -> success
 */
/*******************************************************************************************************/
/*******************************************************************************************************/
// create a fops struct
/*
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

 * fops - File operations structure for the character device
 *
 * This structure maps user-space system calls (open, read, write, close)
 * to corresponding driver functions in kernel space.
 *
 * When a user application performs operations on the device file
 * (e.g., /dev/cdac_dev), the kernel internally calls these functions.
 *
 * Members:
 *  owner   : Points to THIS_MODULE
 *            Prevents the module from being unloaded while in use.
 *
 *  open    : Called when device file is opened
 *            → maps to my_open()
 *
 *  read    : Called when user reads from device
 *            → maps to my_read()
 *
 *  write   : Called when user writes to device
 *            → maps to my_write()
 *
 *  release : Called when device file is closed
 *            → maps to my_release()
 */

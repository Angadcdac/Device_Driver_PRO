/* Basic kernel module skelton */

#define pr_fmt(fmt)  KBUILD_MODNAME ": "fmt

// HEADER FILES
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/errno.h>

// MICRO DEFINE
//#define MY_MAJOR_NUM (332)
#define MY_CLS_NAME "cls_name"
#define MY_DEV_NAME "dev_pradeep"

// GLOBAL VARIABLES
//dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
dev_t dev = 0;
static char *str = "Pradeep Vishwakarma";
static int myvar = 5;
static int timer = 1;
static int expire =0;

//MODULE PARAMETERS
module_param(myvar, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(myvar, "THIS IS MAYVAR VALUE !\n");

// STRUCTURE DECLERATION
static struct timer_list tmr;
static struct class *my_class;
static struct device *my_device;
static struct cdev my_cdev;

// FUNCTION PROTOTYPE
void timer_callback(struct timer_list *tmr);
static int __init my_init_fun(void);
static ssize_t my_open(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *fp, char __user *arg, size_t len, loff_t *off);
static ssize_t my_write(struct file *fp, const char __user *arg, size_t len, loff_t *off);
static ssize_t my_release(struct inode *inode, struct file *file);
static void __exit my_exit_fun(void);

// TIMER FUNCTION DEFINITION
void timer_callback(struct timer_list *tmr)
{
	int ctx = in_interrupt();
	pr_info("IN TIMER CALLBACK, CALLED BY %s\n",(ctx==0?"KERNEL":"INTERRUPT"));
	expire =1;
	return;
}

// FUNCTION DEFINITION
static ssize_t my_open(struct inode *inode, struct file *file)
{
	pr_info(" %s IS CALLED \n",__func__);
	return 0;
}

// FUNCTION DEFINITION
static ssize_t my_read(struct file *file, char __user *arg, size_t len, loff_t *off)
{
	pr_info(" %s IS CALLED \n",__func__);
	if(copy_to_user(arg,&expire,sizeof(int))){
		pr_info("KERNEL TO USER SPACE WRITING FAILED\n");
		return -1;
	}
	expire = 0;
	return 0;
}

// FUNCTION DEFINITION
static ssize_t my_write(struct file *file, const char __user *arg, size_t len, loff_t *off)
{
	pr_info(" %s IS CALLED \n",__func__);
	return 0;
}

// FUNCTION DEFINITION
static ssize_t my_release(struct inode *inode, struct file *file)
{
	pr_info(" %s IS CALLED \n",__func__);
	return 0;
}

// FILE OPERATION INITIALIZATION
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

// INIT FUNCTION DECLERATION
static int __init my_init_fun(void)
{
	int ans;
	timer_setup(&tmr, timer_callback, 0);
	pr_info("INIT FUNCTION STARTED! HELLO WORLD\n");
	pr_info("MY STRING IS -> [%s] !\n",str);
	pr_info("MY VAR IS [%d] !\n",myvar);
	timer = myvar*1000;
	mod_timer(&tmr, jiffies+msecs_to_jiffies(timer));
        //ans = register_chrdev_region(dev, 1,MY_DEV_NAME);
        ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
	if ( ans < 0 ){
		pr_info("COULD NOT REGISTERED MAJOR:MINOR !\n");
		return -1;
	}
	pr_info("MAJOR:[%d] AND MINOR:[%d] ALLOCATED!\n",MAJOR(dev),MINOR(dev));
	cdev_init(&my_cdev, &fops);
	ans = cdev_add(&my_cdev, dev, 1);
	if(ans<0){
		pr_info("ADDING DEVICE TO KERNEL FAILED\n");
		goto r_cdev;
	}
	my_class = class_create(THIS_MODULE, MY_CLS_NAME);
	if(IS_ERR(my_class)){
		pr_info("COULD NOT CREATE CLASS %s\n",MY_CLS_NAME);
		goto r_class;
	}
	pr_info("CLASS IS [%s] CREATED!\n",MY_CLS_NAME);
	my_device = device_create(my_class, NULL, dev, NULL, MY_DEV_NAME);
	if(IS_ERR(my_device)){
		pr_info("COULD NOT CREATE DEVICE %s\n",MY_DEV_NAME);
		goto r_device;
	}
	pr_info("DEVICE %s IS CREATED UNDER CLASS %s WITH SUCCESS\n",MY_DEV_NAME, MY_CLS_NAME);
 	return 0;
r_device:
	class_destroy(my_class);
r_class:
	cdev_del(&my_cdev);
r_cdev:
	unregister_chrdev_region(dev, 1);
	return -1;
}

// EXIT FUNCTION DECLERATION
static void __exit my_exit_fun(void)
{
	pr_info("EXIT FUNCTION END!\n");
	del_timer(&tmr);
	device_destroy(my_class, dev);
	class_destroy(my_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("THE VARIABLE IS [%d] !\n",myvar);
	pr_info("GOODBYE WORLD!\n");
	return;
}

// MODULE LADDER
module_init(my_init_fun);
module_exit(my_exit_fun);

// METADATA
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Hello world module!");

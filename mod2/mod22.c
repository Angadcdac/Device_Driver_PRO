/* Illustrate module parameters with callbacks */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

// instantiation with default, overriden by insmod if any
static short myshort = 100;

int notify_on_set(const char *val, const struct kernel_param *kp)
{
	int ans = param_set_short(val, kp);
	if (ans==0)
	{
		pr_info("Callback function %s called\n", __func__);
		pr_info("New value of myshort = %d\n", myshort);
		return 0;
	}
	return -1;
}

const struct kernel_param_ops myops = 
{
	
	// my function - overrides default
        // called when value is written
	.set = &notify_on_set,
        // called when value is read	 
	// kernel's function - default
	.get = &param_get_short,
};

// perms: all can read, only USR (root) can write/edit
module_param_cb(myshort, &myops, &myshort, S_IRUGO | S_IWUSR);

// create MODULE DESC strings for modinfo
MODULE_PARM_DESC(myshort, "This is a short-int variable with callback");

static int __init my_mod_init(void)
{
	pr_info("Hello from mod22!\n");
	pr_info("myshort = %d\n", myshort);
	return 0;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye from mod22!\n");

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module with param callback support!");
/****************************************************************************************/
// Callback function triggered when parameter value is changed via sysfs
// Callback function that is triggered when the parameter is modified via sysfs
// Example trigger: echo 50 > /sys/module/<module>/parameters/myshort
/*
int notify_on_set(const char *val, const struct kernel_param *kp)
{
        // Convert input string (val) to short and assign it to 'myshort'
        // Also validates the input value
        int ans = param_set_short(val, kp);

        // Check if conversion and assignment were successful
        if (ans == 0)
        {
                // Print the name of the callback function
                // __func__ automatically expands to "notify_on_set"
                pr_info("Callback function %s called\n", __func__);

                // Print the updated value of the parameter
                pr_info("New value of myshort = %d\n", myshort);

                // Return 0 to indicate success
                return 0;
        }

        // Return -1 if input is invalid or update fails
        return -1;
}
}*/
/****************************************************************************************/
/*********module_param_cb(myshort, &myops, &myshort, S_IRUGO | S_IWUSR);*****************/
// Create a module parameter named 'myshort' with callback support
// myshort  → parameter name visible in /sys/module/<module>/parameters/
// &myops   → pointer to operations (set = callback function, get = read function)
// &myshort → address of the actual variable in kernel
// S_IRUGO  → readable by all users
// S_IWUSR  → writable only by root (user)
// So overall permission ≈ 0644 (read for all, write for root only)
/****************************************************************************************/

/*******MODULE_PARM_DESC(myshort, "This is a short-int variable with callback");*********/
// Add description for this parameter (visible using 'modinfo')
// Helps document what the parameter does
/****************************************************************************************/
/*        Start
          │
          ▼
 Load kernel module (insmod mod22.ko)
          │
          ▼
 Parameter "myshort" created in sysfs
 (/sys/module/mod22/parameters/myshort)
          │
          ▼
 User writes value:
 echo 50 > /sys/module/mod22/parameters/myshort
          │
          ▼
 Kernel receives input ("50")
          │
          ▼
 notify_on_set() callback is triggered
          │
          ▼
 param_set_short(val, kp) called
          │
          ▼
   ┌───────────────┐
   │ Conversion OK?│
   └───────┬───────┘
           │Yes
           ▼
   Update variable (myshort = 50)
           │
           ▼
 Print logs using pr_info()
           │
           ▼
 Return 0 (success)
           │
           ▼
         End

           │No
           ▼
 Return -1 (error)
           │
           ▼*/

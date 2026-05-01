/* Kernel high res timer */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

#define TIME_INTVL_SECS		(0)		// seconds
#define TIME_INTVL_NSECS	(1000000000UL) 	// nanoseconds

// define a kernel timer
static struct hrtimer my_hrtimer;

static int count = 0;

// callback function for the kernel HR timer
enum hrtimer_restart my_timer_cb(struct hrtimer *hrtmr)
{
	pr_info("Inside HR timer! val=[%d]\n", count++);

	// forward HR timer
	hrtimer_forward_now(hrtmr, ktime_set(TIME_INTVL_SECS, TIME_INTVL_NSECS));
	
	return HRTIMER_RESTART;
}

static int __init my_mod_init(void)
{
	ktime_t ktime;

	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	
	ktime = ktime_set(TIME_INTVL_SECS, TIME_INTVL_NSECS);
	hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	my_hrtimer.function = my_timer_cb;
	pr_info("Kernel HR timer created!\n");

	hrtimer_start(&my_hrtimer, ktime, HRTIMER_MODE_REL);

	return 0;
}

static void __exit my_mod_exit(void)
{
	hrtimer_cancel(&my_hrtimer);
	pr_info("Deleted kernel HR timer!\n");
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel's high resolution timer!");
/************************************************************************************/
/*
// High-resolution timer callback function
// This function is called when the hrtimer expires
enum hrtimer_restart my_timer_cb(struct hrtimer *hrtmr)
{
        // Print message with incrementing counter value
        // 'count' is assumed to be a global/static variable
        pr_info("Inside HR timer! val=[%d]\n", count++);

        // Move (forward) the timer expiry time to the next interval
        // This is REQUIRED for periodic timers
        // ktime_set(sec, nsec) sets the time interval
        // TIME_INTVL_SECS  -> seconds part
        // TIME_INTVL_NSECS -> nanoseconds part
        hrtimer_forward_now(hrtmr, ktime_set(TIME_INTVL_SECS, TIME_INTVL_NSECS));

        // Return HRTIMER_RESTART to restart the timer
        // Makes this timer behave like a periodic timer
        return HRTIMER_RESTART;
*/

/*************************************************************************************/
// Module initialization function
// This function is called when the module is loaded using insmod
/*
static int __init my_mod_init(void)
{
        // ktime_t is used for high-resolution time representation
        ktime_t ktime;

        // Print module load message
        // KBUILD_MODNAME gives the module name
        pr_info("Hello world from %s!\n", KBUILD_MODNAME);

        // Create time interval using seconds and nanoseconds
        // This defines when the timer will expire first time
        ktime = ktime_set(TIME_INTVL_SECS, TIME_INTVL_NSECS);

        // Initialize the high-resolution timer
        // CLOCK_MONOTONIC -> time since system boot (not affected by system time changes)
        // HRTIMER_MODE_REL -> relative time (from now)
        hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

        // Assign callback function to the timer
        // This function will be called when timer expires
        my_hrtimer.function = my_timer_cb;

        // Confirmation log
        pr_info("Kernel HR timer created!\n");

        // Start the timer
        // 'ktime' -> initial expiry time
        // HRTIMER_MODE_REL -> start after relative time interval
        hrtimer_start(&my_hrtimer, ktime, HRTIMER_MODE_REL);

        return 0; // return 0 means successful module load
}
*/
/*********************************************************************************/
/*	hrtimer_cancel(&my_hrtimer);
//     Cancel the high-resolution timer
//      This stops the timer if it is active

// If the timer callback is currently running,i
// this function will wait until the callback finishes (synchronization)
 */
/*********************************************************************************/

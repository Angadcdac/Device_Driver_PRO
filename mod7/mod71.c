/* Kernel timer */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

#define TIME_INTVL	(1000)	// milliseconds

// define a kernel timer
static struct timer_list my_timer;

static int count = 0;

// callback function for the kernel timer
void my_timer_cb(struct timer_list *tmr)
{
	int ctx = in_interrupt();
	pr_info("In timer, count=[%d], running in %s context\n", count++,
			(ctx==0?"kernel":"interrupt") );

	// reload timer with same data - to ensure regular timer start-expiry
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIME_INTVL));
	
	return;
}

static int __init my_mod_init(void)
{
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	
	timer_setup(&my_timer, my_timer_cb, 0);
	pr_info("Kernel timer created!\n");

	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIME_INTVL));

	pr_info("Kernel timer started\n");
	
	return 0;
}

static void __exit my_mod_exit(void)
{
	del_timer(&my_timer);
	pr_info("Deleted kernel timer!\n");
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel timer!");
//========================================================================================/
// void my_timer_cb(struct timer_list *tmr)
/*
 * my_timer_cb - Kernel timer callback function
 *
 * This function is automatically invoked when the kernel timer expires.
 * It runs in soft interrupt (softirq) context, so it must not perform
 * any blocking operations (like sleep or waiting).
 *
 * Typical responsibilities:
 *  - Perform lightweight, non-blocking work
 *  - Update counters or status
 *  - Restart the timer using mod_timer() if periodic behavior is needed
 *
 * @tmr: Pointer to the timer_list structure associated with this callback
 */
//======================================================================================/
//int ctx = in_interrupt();
//========================================================================================/
/*
 * mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIME_INTVL));
 * Check whether current execution is in interrupt context (returns 1)
 * or process/kernel context (returns 0)
 *//*
 * Start or re-arm the kernel timer to expire after TIME_INTVL milliseconds.
 *
 * jiffies                     → current kernel time (in ticks)
 * msecs_to_jiffies(...)      → converts milliseconds to kernel ticks
 * jiffies + delay            → sets future expiry time
 *
 * If the timer is not active:
 *      → it starts the timer
 * If the timer is already active:
 *      → it updates (reschedules) the expiry time
 *
 * When the expiry time is reached, the kernel calls the
 * associated callback function (my_timer_cb).
 *
 * This function is also used inside the callback to make
 * the timer run periodically (restarting it again and again).
 */
//========================================================================================/
/*
 * timer_setup(&my_timer, my_timer_cb, 0);
 * Initialize and set up the kernel timer by associating it with the callback function
 */
//========================================================================================/
/* mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIME_INTVL));
// Start or re-arm the timer to expire after TIME_INTVL milliseconds from 
// current time (jiffies)i
*/
//========================================================================================/

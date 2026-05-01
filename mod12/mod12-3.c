/* Kernel global workqueue - dynamic example */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>       
#include <linux/kobject.h>   
#include <linux/interrupt.h>
#include <linux/workqueue.h>
	
// button related
static unsigned int gpioButton = 46; // P8_16
static unsigned int irqNumber;	     // mapped to gpioButton
static unsigned int numPresses = 0;

// workqueue related
static struct work_struct my_work;

static void my_work_func(struct work_struct *work)
{
   numPresses++;
   pr_info("In %s: numPresses = %d\n", __func__, numPresses);
}

static irq_handler_t button_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
   pr_info("In button press interrupt!\n");

   pr_info("Scheduling work...\n");
   schedule_work(&my_work);

   pr_info("Exiting interrupt handler\n");
   
   return (irq_handler_t) IRQ_HANDLED;
}

static int __init my_init(void){
   int result = 0;

   pr_info("Setting up IRQ for GPIO %d for button\n", gpioButton);

   if (!gpio_is_valid(gpioButton))
   {
      pr_err("Invalid GPIO for button!\n");
      return -ENODEV;
   }

   gpio_request(gpioButton, "sysfs"); 
   gpio_direction_input(gpioButton);  
   irqNumber = gpio_to_irq(gpioButton); 
   pr_info("GPIO %d mapped to IRQ number %d\n", gpioButton, irqNumber);

   gpio_export(gpioButton, false);  		// export in /sys/class/gpio/...

   INIT_WORK(&my_work, my_work_func);   //Dynamic creation of the workqueue

   result = request_irq(irqNumber, 
		   	(irq_handler_t) button_handler, 
			IRQF_TRIGGER_RISING,
			"my_button_handler",
			NULL);

   return result;
}

static void __exit my_exit(void)
{
   free_irq(irqNumber, NULL);
   gpio_unexport(gpioButton);                  
   gpio_free(gpioButton);                      
   pr_info("GPIO %d freed up\n", gpioButton);

   pr_info("%d button presses were detected!\n", numPresses);
   pr_info("Good bye from %s!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Kernel global workqueue - dynamic example");
/*
 * STATIC vs DYNAMIC WORKQUEUE (Linux Kernel)
 *
 * 1. STATIC WORKQUEUE:
 * --------------------
 * - Uses kernel’s global/shared workqueue
 * - No separate worker thread created
 * - Uses schedule_work()
 *
 * Example:
 *      DECLARE_WORK(my_work, my_work_func);
 *      schedule_work(&my_work);
 *
 * Features:
 * - Simple to use
 * - Less control
 * - Shared with other kernel tasks
 * - Suitable for small/lightweight work
 *
 *
 * 2. DYNAMIC WORKQUEUE:
 * ---------------------
 * - Creates a dedicated/private workqueue
 * - Uses create_workqueue() or alloc_workqueue()
 * - Uses queue_work()
 *
 * Example:
 *      my_wq = create_workqueue("my_queue");
 *      queue_work(my_wq, &my_work);
 *
 * Features:
 * - More control over execution
 * - Dedicated worker thread
 * - Better for heavy or critical tasks
 * - Slightly higher resource usage
 *
 *
 * SUMMARY:
 * --------
 * Static  -> Simple, shared, lightweight
 * Dynamic -> Flexible, private, powerful
 */
/**************************************************************************************************/
/*
 * TASKLET vs WORKQUEUE (Linux Kernel)
 *
 * 1. TASKLET:
 * ----------
 * - Runs in softirq context (bottom half)
 * - Cannot sleep or block
 * - Executes very fast operations
 * - Runs on same CPU where it was scheduled
 * - Good for quick deferred work after interrupt
 *
 * Example:
 *      tasklet_schedule(&my_tasklet);
 *
 * Use cases:
 * - Small calculations
 * - Quick state updates
 * - Simple interrupt handling tasks
 *
 *
 * 2. WORKQUEUE:
 * ------------
 * - Runs in process context (kernel thread)
 * - Can sleep and block
 * - Suitable for longer/heavy operations
 * - Can run on different CPU
 *
 * Example:
 *      schedule_work(&my_work);
 *
 * Use cases:
 * - File operations
 * - Memory allocation
 * - Complex processing
 *
 *
 * KEY DIFFERENCE:
 * --------------
 * TASKLET   -> Fast, non-blocking, softirq context
 * WORKQUEUE -> Flexible, can sleep, process context
 */


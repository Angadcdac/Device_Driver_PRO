/* My own workqueue - dynamic example */

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
static struct workqueue_struct *my_work_queue;
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
   queue_work(my_work_queue, &my_work);

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

   my_work_queue = create_workqueue("my_work_queue");
   pr_info("My work queue created!\n");

   INIT_WORK(&my_work, my_work_func);

   result = request_irq(irqNumber, 
		   	(irq_handler_t) button_handler, 
			IRQF_TRIGGER_RISING,
			"my_button_handler",
			NULL);

   return result;
}

static void __exit my_exit(void)
{
   destroy_workqueue(my_work_queue);
   pr_info("Work queue destroyed!\n");

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
MODULE_DESCRIPTION("My own workqueue - dynamic example");
/*************************************************************************************/
// queue_work(my_work_queue, &my_work);

/*
 * my_work_queue (OWN WORKQUEUE):
 * ------------------------------
 * This is a custom workqueue created by *this driver/module* using:
 *
 *     my_work_queue = create_workqueue("my_work_queue");
 *
 * OR (modern way):
 *     alloc_workqueue(...)
 *
 * It is NOT a system default workqueue.
 *
 * It is your own dedicated kernel thread pool used only for this driver.
 *
 * ------------------------------------------------------------
 * What "own workqueue" means:
 * ---------------------------
 * - It belongs only to this driver/module
 * - Kernel creates a separate worker thread for it
 * - Your work items run in that private thread
 * - It is NOT shared with other drivers (unlike system_wq)
 *
 * ------------------------------------------------------------
 * &my_work:
 * ----------
 * This is the actual task (work item) you want to execute later.
 * It contains:
 *   - pointer to function (my_work_func)
 *   - execution state (pending/running/done)
 *
 * ------------------------------------------------------------
 * queue_work():
 * -------------
 * This function submits your work item to YOUR workqueue.
 *
 * Flow:
 *   IRQ → queue_work() → kernel worker thread → my_work_func()
 *
 * IMPORTANT:
 * ----------
 * If my_work is already queued or running,
 * this call will be ignored (no duplicate execution).
 *
 * ------------------------------------------------------------
 * SUMMARY:
 * --------
 * my_work_queue = your private background kernel thread system
 * my_work       = task to execute
 * queue_work()  = send task to your background worker
 */

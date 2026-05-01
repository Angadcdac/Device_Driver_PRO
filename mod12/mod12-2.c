/* Kernel global workqueue - static example */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>       
#include <linux/kobject.h>   
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/atomic.h>

// button related
static unsigned int gpioButton = 46; // P8_16
static unsigned int irqNumber;	     // mapped to gpioButton
static unsigned int numPresses = 0;

// workqueue related
static void my_work_func(struct work_struct *work)
{
   numPresses++;
   pr_info("In %s: numPressed = %d\n", __func__, numPresses);
   pr_info("val = %ld\n", atomic_long_read(&(work->data)));
}
DECLARE_WORK(my_work, my_work_func);//Static Workqueue creation

static irq_handler_t button_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
  // long i;
   pr_info("In button press interrupt!\n");

   pr_info("Scheduling work...\n");
  // atomic_long_set(&my_work.data, 100);
  // i = atomic_long_read(&my_work.data);
//   pr_info("i=%ld\n", i);

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
MODULE_DESCRIPTION("Kernel global workqueue - static example");

/*
 * Workqueue handler function (Bottom Half).
 *
 * This function is executed by the kernel worker
 * thread after the work item is scheduled using
 * schedule_work().
 *
 * Purpose:
 * 1. Perform deferred work outside interrupt context
 * 2. Increment button press counter
 * 3. Print debugging information
 *
 * Parameter:
 *      work -> pointer to the associated work_struct
 *
 * NOTE:
 * Workqueues execute in process context, so they
 * are allowed to sleep and perform longer operations.
 */

//static void my_work_func(struct work_struct *work)
//{
   /* Increment total button press count 
   numPresses++;

   Print current function name and press count 
   pr_info("In %s: numPressed = %d\n", __func__,numPresses);
   
    * Read internal workqueue data field.
    *
    * NOTE:
    * work->data is managed internally by the kernel
    * and should normally not be accessed directly
    * in production drivers.
    

   pr_info("val = %ld\n",
            atomic_long_read(&(work->data)));
}


 * Declare and initialize a static workqueue item.
 *
 * my_work         -> workqueue object
 * my_work_func() -> function executed when work runs
 *
 * This creates a work item that can later be
 * scheduled using:
 *
 *      schedule_work(&my_work);
 

DECLARE_WORK(my_work, my_work_func);*/
/***************************************************************************************************/
/*
 * Schedule the workqueue job for execution.
 *
 * This function queues the work item into the
 * kernel's global workqueue.
 *
 * After scheduling:
 *      my_work_func() will execute later
 *      in process context by a kernel
 *      worker thread.
 *
 * Purpose:
 * 1. Keep interrupt handler short and fast
 * 2. Defer processing to bottom half
 * 3. Allow longer operations outside ISR
 *
 * NOTE:
 * If the work item is already pending,
 * schedule_work() will not queue it again.
 */





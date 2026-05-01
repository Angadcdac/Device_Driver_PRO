/* Interrupt handling using GPIO */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>       
#include <linux/kobject.h>   
#include <linux/interrupt.h>
		
// button related
static unsigned int gpioButton = 46; // P8_16
static unsigned int irqNumber;	     // mapped to gpioButton
static unsigned int numPresses = 0;

static irq_handler_t button_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
   pr_info("Button press interrupt!\n");
   numPresses++;

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
MODULE_DESCRIPTION("A simple interrupt driver using GPIO button");
/********************************************************************************************/
/*
 * button_handler()
 *
 * Interrupt Service Routine (ISR) for the button GPIO.
 *
 * This function is automatically called by the kernel
 * whenever the configured interrupt occurs on GPIO46.
 *
 * Trigger condition:
 *      Rising edge interrupt
 *      (LOW -> HIGH transition)
 *
 * Operations performed:
 * 1. Print interrupt message to kernel log
 * 2. Increment button press counter
 * 3. Inform kernel that interrupt was handled
 *
 * NOTE:
 * Interrupt handlers should execute very quickly.
 * Avoid sleeping, long loops, or blocking operations.
 *

static irq_handler_t button_handler(unsigned int irq,
                                    void *dev_id,
                                    struct pt_regs *regs)
{
   // Print message in kernel log
   pr_info("Button press interrupt!\n");

   // Increment number of button presses
   numPresses++;

   // Notify kernel that interrupt was handled successfully
   return (irq_handler_t) IRQ_HANDLED;
}
*/

/*
 * GPIO and Interrupt Configuration
 *
 * 1. Request ownership of the GPIO pin
 *    from the kernel GPIO subsystem.
 *
 * 2. Configure GPIO pin as INPUT because
 *    it is connected to a push button.
 *
 * 3. Convert GPIO number to IRQ number
 *    so it can be used by the interrupt subsystem.
 *
 * 4. Export GPIO to sysfs:
 *      /sys/class/gpio/gpio46/
 *
 * 5. Register interrupt handler using request_irq().
 *
 *    Interrupt trigger type:
 *      IRQF_TRIGGER_RISING
 *
 *    Meaning:
 *      Interrupt occurs on LOW -> HIGH transition
 *      when the button is pressed.
 *

gpio_request(gpioButton, "sysfs");

// Configure GPIO as input pin 
gpio_direction_input(gpioButton);

// Map GPIO pin to IRQ number 
irqNumber = gpio_to_irq(gpioButton);

pr_info("GPIO %d mapped to IRQ number %d\n",
         gpioButton,
         irqNumber);

// Export GPIO in sysfs 
gpio_export(gpioButton, false);

// Register interrupt handle
result = request_irq(
            irqNumber,
            (irq_handler_t) button_handler,
            IRQF_TRIGGER_RISING,
*/
/**************************************************************************************************/

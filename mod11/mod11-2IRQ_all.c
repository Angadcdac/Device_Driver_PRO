/* LED control and blinking via GPIO */

/*
$make ARCH=arm CROSS_COMPILE=gnueabihf-
$scp *.ko debian@beaglebone.local:~/.

$sudo su
$sudo insmod mod11-2_all.ko

$echo 13 > /sys/cdac_led/led47/data4b //echo don't support sudo
$echo 100 > /sys/cdac_led/led47/blinkPeriod
$echo on > /sys/cdac_led/led47/mode 
$echo flash > /sys/cdac_led/led47/mode 
$echo off > /sys/cdac_led/led47/mode 

$cat /sys/cdac_led/led47/data4b

$sudo rmmode mod11-2_all
*/

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>       
#include <linux/kobject.h>    
#include <linux/kthread.h>    
#include <linux/delay.h>
#include <linux/interrupt.h>      

static struct kobject *kobj_ref;            
static struct task_struct *task;            

//BUTTON				    
static unsigned int gpioButton = 46; // P8_16
static unsigned int irqNumber;	     // mapped to gpioButton
//LED
//static unsigned int numPresses = 0;

static unsigned int gpioLED[4] = {47, 48, 49, 60};         

static unsigned int data4b = 5; 
//module_param(data4b, byte, S_IRUGO | S_IWUSR);   

static unsigned int blinkPeriod = 100;     // in msecs
module_param(blinkPeriod, uint, S_IRUGO | S_IWUSR);   
MODULE_PARM_DESC(blinkPeriod, "LED blink period in ms (min=1, default=1000, max=10000)");

static char ledName[10] = {0};
static bool ledOn = 0;                      
enum modes { OFF, ON, FLASH };          // LED modes    
static enum modes mode = FLASH;		// default LED mode

static ssize_t mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
   switch(mode){
      case OFF:   return sprintf(buf, "off\n");       
      case ON:    return sprintf(buf, "on\n");
      case FLASH: return sprintf(buf, "flash\n");
      default:    return sprintf(buf, "error\n"); 
   }
}

static ssize_t mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
   if (strncmp(buf,"on",count-1)==0) { mode = ON; }   
   else if (strncmp(buf,"off",count-1)==0) { mode = OFF; }
   else if (strncmp(buf,"flash",count-1)==0) { mode = FLASH; }

   return count;
}

static ssize_t period_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
   return sprintf(buf, "%d\n", blinkPeriod);
}

static ssize_t period_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
   unsigned int period;                    

   sscanf(buf, "%du", &period);             
   if ((period>1)&&(period<=10000)){        
      blinkPeriod = period;                 
   }
   pr_info("Period set to %d\n", blinkPeriod);

   return sizeof(period);
}

static ssize_t data_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
   return sprintf(buf, "%d\n", data4b);
}

static ssize_t data_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned char d4b = 0;
	sscanf(buf, "%hhu", &d4b);
	if ((d4b<16)){        
	      data4b = d4b;                 
       }
       pr_info("data set to %d\n", data4b);
       
       return sizeof(data4b);
}

static struct kobj_attribute period_attr = __ATTR(blinkPeriod, 0664, period_show, period_store);
static struct kobj_attribute mode_attr = __ATTR(mode, 0664, mode_show, mode_store);
static struct kobj_attribute data4b_attr = __ATTR(data4b, 0664, data_show, data_store);

static struct attribute *led_attrs[] = {
   &period_attr.attr,                       
   &mode_attr.attr,   
   &data4b_attr.attr,//new                      
   NULL,
};

static struct attribute_group attr_group = {
   .name  = ledName,                        
   .attrs = led_attrs,                      
};

static irq_handler_t button_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
   pr_info("Button press interrupt!\n");
   /*numPresses++;
	if ((numPresses<16)){        
	      data4b = numPresses;                 
       }*/
   data4b = ((data4b + 1) % 16);
   return (irq_handler_t) IRQ_HANDLED;
}

static int flash_led(void *arg)
{
	int i = 0;
	uint8_t temp = 0;
	//unsigned char d4b = data4b;
   pr_info("LED flashing thread started with period=%d\n", blinkPeriod);

   while(!kthread_should_stop()){
   	if (mode==FLASH) {ledOn = !ledOn;}
	   else if (mode==ON) ledOn = true;
	   else ledOn = false;          

     for(i = 0; i < 4; i++) {
	temp = (((data4b >> i) & 0x01) & ledOn);
      	gpio_set_value(gpioLED[i], temp);
      	if(((!(temp & i)) || temp) && mode == FLASH)
      		msleep(blinkPeriod/2);
      }      
   }

   pr_info("LED Flashing thread exiting\n");
   return 0;
}

static int __init my_init(void){
   int result = 0;
   int i = 0;
   sprintf(ledName, "led%d", gpioLED[0]);  
   pr_info("Init'ing GPIO LED %s...\n", ledName);

   kobj_ref = kobject_create_and_add("cdac_led", NULL); // kernel_kobj points to /sys/cdac_edd
   if(!kobj_ref){
      pr_err("Failed to create kobject\n");
      return -ENOMEM;
   }

   // create an attr group
   result = sysfs_create_group(kobj_ref, &attr_group);
   if(result) {
      pr_err("Failed to create sysfs group\n");
      kobject_put(kobj_ref);                
      return result;
   }

   ledOn = true;
   for(i = 0; i < 4; i++) {
	   gpio_request(gpioLED[i], "sysfs");          
	   gpio_direction_output(gpioLED[i], ledOn);   
	   gpio_export(gpioLED[i], false);  		// export in /sys/class/gpio/...
   }
   
   //SETTING SWITCH
   pr_info("Setting up IRQ for GPIO %d for button\n", gpioButton);

   if (!gpio_is_valid(gpioButton))
   {
      pr_err("Invalid GPIO for button!\n");
      return -ENODEV;
   }
   gpio_request(gpioButton, "sysfs"); 
   gpio_direction_input(gpioButton);
   gpio_set_debounce(gpioButton, 500);
   irqNumber = gpio_to_irq(gpioButton); 
   pr_info("GPIO %d mapped to IRQ number %d\n", gpioButton, irqNumber);
   gpio_export(gpioButton, false);  		// export in /sys/class/gpio/...
   result = request_irq(irqNumber, 
		   	(irq_handler_t) button_handler, 
			IRQF_TRIGGER_RISING,
			"my_button_handler",
			NULL);
   
   task = kthread_run(flash_led, NULL, "LED blink thread"); 
   if(IS_ERR(task))
   {                                     
      pr_err("Failed to create LED flashing task\n");
      return PTR_ERR(task);
   }
   return result;
}

static void __exit my_exit(void)
{
	int i = 0;
   kthread_stop(task);            
   kobject_put(kobj_ref);                  

   for(i = 0; i < 4; i++) {
	   gpio_set_value(gpioLED[i], 0);              
	   gpio_unexport(gpioLED[i]);                  
	   gpio_free(gpioLED[i]);  
                      
   	pr_info("GPIO %d freed up\n", gpioLED[i]);
   }
   free_irq(irqNumber, NULL);
   gpio_unexport(gpioButton);                  
   gpio_free(gpioButton);           

   printk(KERN_INFO "Good bye from %s!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("A simple LED driver using GPIOs");

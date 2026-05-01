/* LED control and blinking via GPIO */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>       
#include <linux/kobject.h>    
#include <linux/kthread.h>    
#include <linux/delay.h>      
#include <linux/moduleparam.h>

#define SIZE (4)

static struct kobject *kobj_ref;            
static struct task_struct *task;            
static unsigned int gpioLED[SIZE] = {0};					    
/*static unsigned int gpioLED = 60;          
static unsigned int gpioLED1 = 48;          
static unsigned int gpioLED2 = 49;          
static unsigned int gpioLED3 = 47;*/          
//static unsigned int gpioLED[4] = {60, 48, 49, 47};                  

static unsigned int count=0;
static unsigned int blinkPeriod = 100;     // in msecs
module_param_array(gpioLED, int, &count, S_IRUGO | S_IWUSR); // copy code on BBB and write[sudo insmod self_mod11-1.ko gpioLED=60,48,49,47]      
module_param(blinkPeriod, uint, S_IRUGO | S_IWUSR);   
MODULE_PARM_DESC(blinkPeriod, "LED blink period in ms (min=1, default=1000, max=10000)");

static unsigned int i=0;
//static unsigned int j=0;
static unsigned int x=0;
static unsigned int y=0;
static unsigned int data=0;
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
   return sprintf(buf, "%d\n", data);
}

static ssize_t data_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
   unsigned int d;                    

   sscanf(buf, "%du", &d);             
   if ((d<16)){        
      data = d;                 
   }
   pr_info("data set to %d\n", data);

   return sizeof(data);
}

static struct kobj_attribute period_attr = __ATTR(blinkPeriod, 0664, period_show, period_store);
static struct kobj_attribute mode_attr = __ATTR(mode, 0664, mode_show, mode_store);
static struct kobj_attribute data_attr = __ATTR(data, 0664, data_show, data_store);

static struct attribute *led_attrs[] = {
   &period_attr.attr,                       
   &mode_attr.attr,
   &data_attr.attr,                         
   NULL,
};

static struct attribute_group attr_group = {
   .name  = ledName,                        
   .attrs = led_attrs,                      
};

static int flash_led(void *arg)
{
	uint8_t d;
   pr_info("LED flashing thread started with period=%d\n", blinkPeriod);

   while(!kthread_should_stop()){          
      if (mode==FLASH) ledOn = !ledOn;  
      else if (mode==ON) ledOn = true;
      else ledOn = false;

      /*gpio_set_value(gpioLED, ledOn);      
      gpio_set_value(gpioLED1, ledOn);      
      gpio_set_value(gpioLED2, ledOn);      
      gpio_set_value(gpioLED3, ledOn);*/ //instead of this we used for loop
      for(i=0;i<4;i++){
      	d = ((data >> i) & 0x01)&ledOn;
	      gpio_set_value(gpioLED[i], d);
      }
					   
      msleep(blinkPeriod/2);                
   }

   pr_info("LED Flashing thread exiting\n");
   return 0;
}

static int __init my_init(void){
   int result = 0;

   /*sprintf(ledName, "led%d", gpioLED);  
   sprintf(ledName, "led%d", gpioLED1);  
   sprintf(ledName, "led%d", gpioLED2);  
   sprintf(ledName, "led%d", gpioLED3); */ //instead of this we used for loop
   
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
   for(x=0;x<4;x++){
	   ledOn = true;
   	   gpio_request(gpioLED[x], "sysfs");          
   	   gpio_direction_output(gpioLED[x], ledOn);   
   	   gpio_export(gpioLED[x], false);	   // export in /sys/class/gpio/...
   }
   //instead of this we used for loop at the top
   /*ledOn = true;
   gpio_request(gpioLED1, "sysfs");          
   gpio_direction_output(gpioLED1, ledOn);   
   gpio_export(gpioLED1, false);  		// export in /sys/class/gpio/...

   ledOn = true;
   gpio_request(gpioLED2, "sysfs");          
   gpio_direction_output(gpioLED2, ledOn);   
   gpio_export(gpioLED2, false);  		// export in /sys/class/gpio/...

   ledOn = true;
   gpio_request(gpioLED3, "sysfs");          
   gpio_direction_output(gpioLED3, ledOn);   
   gpio_export(gpioLED3, false);*/  		// export in /sys/class/gpio/...

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
   kthread_stop(task);            
   kobject_put(kobj_ref);                  
   for(y=0;y<4;y++){
	   gpio_set_value(gpioLED[y], 0);              
   	   gpio_unexport(gpioLED[y]);                  
   	   gpio_free(gpioLED[y]);                      
	   pr_info("GPIO %d freed up\n", gpioLED[y]);
   }
   // instead of this we used for loop at the top
   /*gpio_set_value(gpioLED, 0);              
   gpio_set_value(gpioLED1, 0);              
   gpio_set_value(gpioLED2, 0);              
   gpio_set_value(gpioLED3, 0);              
   gpio_unexport(gpioLED);                  
   gpio_unexport(gpioLED1);                  
   gpio_unexport(gpioLED2);                  
   gpio_unexport(gpioLED3);                  
   gpio_free(gpioLED);                      
   gpio_free(gpioLED1);                      
   gpio_free(gpioLED2);                      
   gpio_free(gpioLED3);                      
   pr_info("GPIO %d freed up\n", gpioLED);
   pr_info("GPIO %d freed up\n", gpioLED1);
   pr_info("GPIO %d freed up\n", gpioLED2);
   pr_info("GPIO %d freed up\n", gpioLED3);*/

   printk(KERN_INFO "Good bye from %s!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("A simple LED driver using GPIOs");


/*
root@BeagleBone:/home/debian# ls
data  mod11-1.ko  mod11-2.ko  self_mod11-1.ko
root@BeagleBone:/home/debian# sudo rmmod self_mod11-1.ko 
root@BeagleBone:/home/debian# sudo insmod self_mod11-1.ko blinkPeriod=1000 gpioLED=60,48,49,47
root@BeagleBone:/home/debian# echo 1 > /sys/cdac_led/led60/data 
root@BeagleBone:/home/debian# echo 2 > /sys/cdac_led/led60/data 
root@BeagleBone:/home/debian# echo 3 > /sys/cdac_led/led60/data 
root@BeagleBone:/home/debian# echo 4 > /sys/cdac_led/led60/data 
root@BeagleBone:/home/debian# echo 8 > /sys/cdac_led/led60/data 
root@BeagleBone:/home/debian# echo 15 > /sys/cdac_led/led60/data 
root@BeagleBone:/home/debian#
*/

/* Kernel thread basics */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define TIME_INTVL	(1000)	// milliseconds

// define thread struct
static struct task_struct *thread1, *thread2;
static char thread1_name[] = "Thread-1";
static char thread2_name[] = "Thread-2";

int thread_func1(void *pv)
{
	int  thread_1 = 0;
	while(!kthread_should_stop())
	{
		pr_info("In thread %s, thread_1 =%d\n", __func__, thread_1++);
		msleep(TIME_INTVL);
	}
	return 0;
}

int thread_func2(void *pv)
{
	int thread_2 = 0;
	while(!kthread_should_stop())
	{
		pr_info("In thread %s, thread 2 =%d\n", __func__, thread_2++);
		msleep(TIME_INTVL*2);
	}
	return 0;
}

static int __init my_mod_init(void)
{
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);

	thread1 = kthread_run(thread_func1, NULL, thread1_name);
	if (thread1)
		pr_info("Thread %s created!\n", thread1_name);
	else
	{
		pr_err("Cannot create thread %s\n", thread1_name);
		goto r_thread1;
	}

	thread2 = kthread_run(thread_func2, NULL, thread2_name);
	if (thread2)
		pr_info("Thread %s created!\n", thread2_name);
	else
	{
		pr_err("Cannot create thread %s\n", thread2_name);
		goto r_thread2;
	}
	return 0;

r_thread2:
	kthread_stop(thread1);
r_thread1:
	return -1;
}

static void __exit my_mod_exit(void)
{
	kthread_stop(thread1);
	kthread_stop(thread2);
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel threads!");
/****************************************************************************************************/
// Create a kernel thread
// thread1 = kthread_run(thread_func1, NULL, thread1_name);

/*
 * kthread_run():
 * - Creates and starts a kernel thread
 * - thread_func1 → function executed by the thread
 * - NULL → argument passed to thread function (void *data)
 * - thread1_name → name of the thread (visible in logs / ps)
 *
 * Return value:
 * - On success → pointer to task_struct (thread descriptor)
 * - On failure → returns ERR_PTR (not NULL, important!)
 


 Check if thread creation is successful
 if (thread1)
        pr_info("Thread %s created!\n", thread1_name);
        *
         * pr_info:
         * - Prints message in kernel log (dmesg)
         * - Confirms thread creation
         *

else
{
        pr_err("Cannot create thread %s\n", thread1_name);
        *
         * pr_err:
         * - Prints error message in kernel log
         *

        goto r_thread1;
        *
         * goto label for cleanup
         * Used when thread creation fails
         * Ensures proper resource handling
         *
}
*/
/******************************************************************************************************/
/* int thread_func1(void *pv)
   {
        int thread_1 = 0;   // Local counter variable for this thread

         *
         * Infinite loop for thread execution
         * Thread will keep running until it is asked to stop
         *
        while (!kthread_should_stop())
        {
                 *
                 * pr_info:
                 * - Prints message to kernel log (visible using dmesg)
                 * - __func__ gives current function name (thread_func1)
                 * - thread_1++ prints value first, then increments
                 *
                 pr_info("In thread %s, thread_1 =%d\n", __func__, thread_1++);

                 *
                * msleep(TIME_INTVL):
                 * - Puts ONLY this thread to sleep for TIME_INTVL milliseconds
                 * - During this time, CPU can run other threads
                 * - Thread state becomes TASK_INTERRUPTIBLE
                 *
                msleep(TIME_INTVL);
        }

         *
         * When kthread_stop() is called from another context,
         * kthread_should_stop() becomes true and loop exits
         *

        return 0;   // Thread exits cleanly
}
*/




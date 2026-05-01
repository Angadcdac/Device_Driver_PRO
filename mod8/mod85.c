/* Kernel threads with spinlock-protected shared variables */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/spinlock.h>

#define TIME_INTVL	(1000)	// milliseconds
#define BUF_SZ		(1024)	// in bytes

// define thread structs
static struct task_struct *thread1, *thread2;
static char thread1_name[] = "Thread-1";
static char thread2_name[] = "Thread-2";

// shared data
static int count = 0;
static char buffer[BUF_SZ] = {0};

// spinlock
DEFINE_SPINLOCK(my_spinlock);

int thread_func1(void *pv)
{
	while(!kthread_should_stop())
	{
		spin_lock(&my_spinlock);
		// !!! WE CANNOT SLEEP WHILE WE HAVE THE SPINLOCK !!!
		/*When you enable the msleep then it stuck in the beagle bone because sleeping is not 
		 * allowed on the spin lock*/
//		msleep(TIME_INTVL);
		if (spin_is_locked(&my_spinlock))
			pr_info("%s got lock!\n", thread1_name);
		memset(buffer, 0, BUF_SZ);
		sprintf(buffer, "%s:%d", thread1_name, count++);
		pr_info("In thread %s, buffer=%s\n", __func__, buffer);
		spin_unlock(&my_spinlock);
	        msleep(TIME_INTVL);
	}
	return 0;
}

int thread_func2(void *pv)
{
	while(!kthread_should_stop())
	{
		spin_lock(&my_spinlock);
		if (spin_is_locked(&my_spinlock))
			pr_info("%s got lock!\n", thread2_name);
		memset(buffer, 0, BUF_SZ);
		sprintf(buffer, "%s:%d", thread2_name, count++);
		pr_info("In thread %s, buffer=%s\n", __func__, buffer);
		spin_unlock(&my_spinlock);
		msleep(TIME_INTVL);
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
MODULE_DESCRIPTION("Module to demo spinlock-protected shared variables!");
/*******************************************************************************************************/
/*
   DEFINE_SPINLOCK(my_spinlock);
     // Declares and initializes a spinlock variable named 'my_spinlock'.
     // This macro internally expands to something like:
     // spinlock_t my_spinlock = __SPIN_LOCK_UNLOCKED(my_spinlock);
     // So no separate declaration (spinlock_t my_spinlock) is required.
*/
/*******************************************************************************************************/
/*
 * spin_lock(&my_spinlock);
     // Acquires the spinlock 'my_spinlock'.
     // If the lock is free → current thread takes it and enters critical section.
     // If the lock is already held → the thread will busy-wait (spin) until it becomes free.
     // Disables preemption on the current CPU to avoid being scheduled out while holding the lock.
     // Must be used only for very short critical sections (no sleeping allowed here).
*/
/*******************************************************************************************************/
/*
  spin_unlock(&my_spinlock);
    // Releases the spinlock 'my_spinlock'.
    // Allows other waiting threads (spinning on this lock) to acquire it.
    // Re-enables preemption on the current CPU.
    // Must always be called after spin_lock() to avoid deadlock.
    // Should be done as quickly as possible after finishing the critical section.
*/
/*******************************************************************************************************/

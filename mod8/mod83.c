/* Kernel threads with unprotected shared variables */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>

#define TIME_INTVL	(1000)	// milliseconds
#define BUF_SZ		(1024)	// in bytes

// define thread structs
static struct task_struct *thread1, *thread2;
static char thread1_name[] = "Thread-1";
static char thread2_name[] = "Thread-2";

// shared data - unprotected
static int count = 0;
static char buffer[BUF_SZ] = {0};

int thread_func1(void *pv)
{
	while(!kthread_should_stop())
	{
		memset(buffer, 0, BUF_SZ);
		msleep(TIME_INTVL);
		sprintf(buffer, "%s:%d", thread1_name, count++);
		msleep(TIME_INTVL);
		pr_info("In thread %s, buffer=%s\n", __func__, buffer);
		msleep(TIME_INTVL);
	}
	return 0;
}

int thread_func2(void *pv)
{
	while(!kthread_should_stop())
	{
		memset(buffer, 0, BUF_SZ);
		sprintf(buffer, "%s:%d", thread2_name, count++);
		pr_info("In thread %s, buffer=%s\n", __func__, buffer);
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
MODULE_DESCRIPTION("Module to demo unprotected shared variables!");
/**************************************************************************************************************************/
/*
 * Two kernel threads (thread_func1 and thread_func2) are running concurrently.
 * Both threads share common global variables: buffer and count.
 * Since there is no synchronization mechanism, these variables are unprotected.
 * The scheduler can switch execution between threads at any time.
 * thread_func1 performs multiple sleeps, so it executes slower.
 * thread_func2 executes faster due to fewer delays.
 * While thread_func1 is sleeping, thread_func2 may modify the buffer.
 * When thread_func1 resumes, it may print already modified buffer data.
 * This leads to incorrect or mixed output (data inconsistency).
 * For example, thread_func1 may print data written by thread_func2.
 * Similarly, both threads increment the same count variable.
 * Since count++ is not atomic, updates may overlap.
 * This can result in skipped or unordered counter values.
 * The behavior is non-deterministic and depends on scheduling.
 * This situation is known as a race condition.
 * Race conditions occur when multiple threads access shared data simultaneously.
 * Without protection, data integrity cannot be guaranteed.
 * Proper synchronization like mutex or spinlock is required.
 * Alternatively, separate buffers can be used for each thread.
 * This ensures correct and predictable execution of threads.
 */



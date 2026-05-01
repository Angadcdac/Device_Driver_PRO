/* Kernel threads with read-write locks */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/rwlock.h>

#define WTIME_INTVL	(1000)	// milliseconds
#define RTIME_INTVL	(1000)	// milliseconds
#define BUF_SZ		(1024)	// in bytes

// define thread structs
static struct task_struct *wthread, *rthread1, *rthread2;
static char wthread_name[] = "WThread";
static char rthread1_name[] = "RThread-1";
static char rthread2_name[] = "RThread-2";

// shared data - wthread writes, rthreads only read
static int count = 0;

// rwlock
static DEFINE_RWLOCK(my_rwlock);

int wthread_func(void *pv)
{
	char buffer[20] = {0};

	while(!kthread_should_stop())
	{
		write_lock(&my_rwlock);
		sprintf(buffer, "%s:%d->%d", wthread_name, count++, count);
		pr_info("In thread %s, got wlock, buffer=%s\n", __func__, buffer);
		write_unlock(&my_rwlock);
		msleep(WTIME_INTVL);
	}
	do_exit(0);
	return 0;
}

int rthread_func(void *pv)
{
	char buffer[20] = {0};
	char *thread_name = (char *)pv;

	while(!kthread_should_stop())
	{
		read_lock(&my_rwlock);
		sprintf(buffer, "%s:%d", thread_name, count);
		pr_info("In thread %s, got rlock, buffer=%s\n", __func__, buffer);
		read_unlock(&my_rwlock);
		msleep(RTIME_INTVL);
	}
	do_exit(0);
	return 0;
}

static int __init my_mod_init(void)
{
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);

	wthread = kthread_run(wthread_func, NULL, wthread_name);
	if (wthread)
		pr_info("Thread %s created!\n", wthread_name);
	else
	{
		pr_err("Cannot create thread %s\n", wthread_name);
		goto r_thread1;
	}

	rthread1 = kthread_run(rthread_func, rthread1_name, rthread1_name);
	if (rthread1)
		pr_info("Thread %s created!\n", rthread1_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread1_name);
		goto r_thread2;
	}

	rthread2 = kthread_run(rthread_func, rthread2_name, rthread2_name);
	if (rthread2)
		pr_info("Thread %s created!\n", rthread2_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread2_name);
		goto r_thread3;
	}
	return 0;

r_thread3:
	kthread_stop(rthread1);
r_thread2:
	kthread_stop(wthread);
r_thread1:
	return -1;
}

static void __exit my_mod_exit(void)
{
	kthread_stop(wthread);
	kthread_stop(rthread1);
	kthread_stop(rthread2);
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo rwlock!");
/******************************************************************************************************/
// static DEFINE_RWLOCK(my_rwlock);
// Declares and initializes a read-write spinlock named 'my_rwlock' (currently commented out).
// 'static' → limits scope to this file only.
// DEFINE_RWLOCK() → creates an rwlock_t and initializes it to unlocked state.
// Used when multiple readers are allowed but writers need exclusive access.
// Since it is commented, no read-write lock is currently being used.
/*****************************************************************************************************/
/*		write_lock(&my_rwlock);
		write_unlock(&my_rwlock);
		read_lock(&my_rwlock);
		read_unlock(&my_rwlock);
		write_lock(&my_rwlock);
// Acquires the write lock on 'my_rwlock'.
// Provides exclusive access → no other readers or writers allowed.
// If lock is busy → current thread will spin (busy-wait) until available.

write_unlock(&my_rwlock);
// Releases the write lock.
// Allows waiting readers or writers to acquire the lock.

read_lock(&my_rwlock);
// Acquires the read lock on 'my_rwlock'.
// Multiple readers can hold this lock simultaneously.
// If a writer holds the lock → this thread will spin until it is released.

read_unlock(&my_rwlock);
// Releases the read lock.
// Must be called after read_lock() to avoid deadlock.
*/

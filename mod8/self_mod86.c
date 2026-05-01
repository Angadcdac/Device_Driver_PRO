/* Kernel threads with read-write locks */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/rwlock.h>

//#define WTIME_INTVL	(1000)	// milliseconds
//#define RTIME_INTVL	(1000)	// milliseconds
#define BUF_SZ		(024)	// in bytes

// define thread structs
static struct task_struct *wthread, *rthread1, *rthread2,*rthread3,*rthread4,*rthread5,*rthread6,*rthread7,*rthread8,*rthread9,*rthread10;
static char wthread_name[] = "WThread";
static char rthread1_name[] = "RThread-1";
static char rthread2_name[] = "RThread-2";
static char rthread3_name[] = "RThread-3";
static char rthread4_name[] = "RThread-4";
static char rthread5_name[] = "RThread-5";
static char rthread6_name[] = "RThread-6";
static char rthread7_name[] = "RThread-7";
static char rthread8_name[] = "RThread-8";
static char rthread9_name[] = "RThread-9";
static char rthread10_name[] = "RThread-10";

// shared data - wthread writes, rthreads only read
static int count = 0;
static int wtime_intvl = 0, rtime_intvl = 0;
//MODULE PARAMETER
module_param(wtime_intvl, int, S_IRUGO | S_IWUSR);
module_param(rtime_intvl, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(wtime_intvl,"This is the write sleep time INTVL");
MODULE_PARM_DESC(rtime_intvl,"This is the read  sleep time INTVL");

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
		//msleep(WTIME_INTVL);
		msleep(wtime_intvl);
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
		//msleep(RTIME_INTVL);
		msleep(rtime_intvl);
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

	rthread2 = kthread_run(rthread_func, rthread1_name, rthread2_name);
	if (rthread2)
		pr_info("Thread %s created!\n", rthread2_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread2_name);
		goto r_thread3;
	}
	rthread3 = kthread_run(rthread_func, rthread3_name, rthread3_name);
	if (rthread3)
		pr_info("Thread %s created!\n", rthread3_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread3_name);
		goto r_thread4;
	}
	rthread4 = kthread_run(rthread_func, rthread4_name, rthread4_name);
	if (rthread4)
		pr_info("Thread %s created!\n", rthread4_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread4_name);
		goto r_thread5;
	}
	rthread5 = kthread_run(rthread_func, rthread5_name, rthread5_name);
	if (rthread5)
		pr_info("Thread %s created!\n", rthread5_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread5_name);
		goto r_thread6;
	}
	rthread6 = kthread_run(rthread_func, rthread6_name, rthread6_name);
	if (rthread6)
		pr_info("Thread %s created!\n", rthread6_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread6_name);
		goto r_thread7;
	}
	rthread7 = kthread_run(rthread_func, rthread7_name, rthread7_name);
	if (rthread7)
		pr_info("Thread %s created!\n", rthread7_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread7_name);
		goto r_thread8;
	}
	rthread8 = kthread_run(rthread_func, rthread8_name, rthread8_name);
	if (rthread8)
		pr_info("Thread %s created!\n", rthread8_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread8_name);
		goto r_thread9;
	}
	rthread9 = kthread_run(rthread_func, rthread9_name, rthread9_name);
	if (rthread9)
		pr_info("Thread %s created!\n", rthread9_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread9_name);
		goto r_thread10;
	}
	rthread10 = kthread_run(rthread_func, rthread10_name, rthread10_name);
	if (rthread10)
		pr_info("Thread %s created!\n", rthread10_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread10_name);
		goto r_thread11;
	}
	return 0;

r_thread11:
	kthread_stop(rthread9);
r_thread10:
	kthread_stop(rthread8);
r_thread9:
	kthread_stop(rthread7);
r_thread8:
	kthread_stop(rthread6);
r_thread7:
	kthread_stop(rthread5);
r_thread6:
	kthread_stop(rthread4);
r_thread5:
	kthread_stop(rthread3);
r_thread4:
	kthread_stop(rthread2);
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
	kthread_stop(rthread3);
	kthread_stop(rthread4);
	kthread_stop(rthread5);
	kthread_stop(rthread6);
	kthread_stop(rthread7);
	kthread_stop(rthread8);
	kthread_stop(rthread9);
	kthread_stop(rthread10);
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo rwlock!");

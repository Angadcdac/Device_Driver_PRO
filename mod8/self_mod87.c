/* Kernel threads with seq locks */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/seqlock.h>

//#define WTIME_INTVL	(250)	// milliseconds
//#define RTIME_INTVL	(1000)	// milliseconds
#define BUF_SZ		(1024)	// in bytes

// define thread structs
static struct task_struct *wthread1,*wthread2,*wthread3,*wthread4,*wthread5, *rthread1,*rthread2,*rthread3,*rthread4,*rthread5;
static char wthread1_name[] = "WThread1";
static char wthread2_name[] = "WThread2";
static char wthread3_name[] = "WThread3";
static char wthread4_name[] = "WThread4";
static char wthread5_name[] = "WThread5";
static char rthread1_name[] = "RThread1";
static char rthread2_name[] = "RThread2";
static char rthread3_name[] = "RThread3";
static char rthread4_name[] = "RThread4";
static char rthread5_name[] = "RThread5";

// shared data - wthread writes, rthread reads
static int count = 0;
static int wtime_intvl = 0, rtime_intvl = 0;

// MODULE PARAMETER
module_param(wtime_intvl, int, S_IRUGO | S_IWUSR);
module_param(rtime_intvl, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(wtime_intvl, "the wtime interval is integer");
MODULE_PARM_DESC(rtime_intvl, "the rtime interval is integer");
// rwlock
static seqlock_t my_seqlock;

int wthread_func(void *pv)
{
	char buffer[20] = {0};
	char *thread_name = (char *)pv;
	while(!kthread_should_stop())
	{
		write_seqlock(&my_seqlock);
		sprintf(buffer, "%s:%d->%d", thread_name, count++, count);
		pr_info("In thread %s, got seqlock, buffer=%s\n", __func__, buffer);
		write_sequnlock(&my_seqlock);
		//msleep(WTIME_INTVL);
		msleep(wtime_intvl);
	}
	return 0;
}

int rthread_func(void *pv)
{
	char buffer[20] = {0};
	char *thread_name = (char *)pv;

	unsigned int seq_no;
	unsigned int read_value;

	while(!kthread_should_stop())
	{
		do
		{
			seq_no = read_seqbegin(&my_seqlock);
			pr_info("Read try - seq_no=%d", seq_no);
			read_value = count;
		} 
		while (read_seqretry(&my_seqlock, seq_no));
		pr_info("Read done - seq_no=%d", seq_no);

		sprintf(buffer, "%s:%d", thread_name, count);
		pr_info("In thread %s, buffer=%s\n", __func__, buffer);
		//msleep(RTIME_INTVL);
		msleep(rtime_intvl);
	}
	return 0;
}

static int __init my_mod_init(void)
{
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);

	seqlock_init(&my_seqlock);

	wthread1 = kthread_run(wthread_func, NULL, wthread1_name);
	if (wthread1)
		pr_info("Thread %s created!\n", wthread1_name);
	else
	{
		pr_err("Cannot create thread %s\n", wthread1_name);
		goto r_thread1;
	}
	wthread2 = kthread_run(wthread_func, NULL, wthread2_name);
	if (wthread2)
		pr_info("Thread %s created!\n", wthread2_name);
	else
	{
		pr_err("Cannot create thread %s\n", wthread2_name);
		goto r_thread2;
	}
	wthread3 = kthread_run(wthread_func, NULL, wthread3_name);
	if (wthread3)
		pr_info("Thread %s created!\n", wthread3_name);
	else
	{
		pr_err("Cannot create thread %s\n", wthread3_name);
		goto r_thread3;
	}
	wthread4 = kthread_run(wthread_func, NULL, wthread4_name);
	if (wthread4)
		pr_info("Thread %s created!\n", wthread4_name);
	else
	{
		pr_err("Cannot create thread %s\n", wthread4_name);
		goto r_thread4;
	}
	wthread5 = kthread_run(wthread_func, NULL, wthread5_name);
	if (wthread5)
		pr_info("Thread %s created!\n", wthread5_name);
	else
	{
		pr_err("Cannot create thread %s\n", wthread5_name);
		goto r_thread5;
	}

	rthread1 = kthread_run(rthread_func, rthread1_name, rthread1_name);
	if (rthread1)
		pr_info("Thread %s created!\n", rthread1_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread1_name);
		goto r_thread6;
	}
	rthread2 = kthread_run(rthread_func, rthread2_name, rthread2_name);
	if (rthread2)
		pr_info("Thread %s created!\n", rthread2_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread2_name);
		goto r_thread7;
	}
	rthread3 = kthread_run(rthread_func, rthread3_name, rthread3_name);
	if (rthread3)
		pr_info("Thread %s created!\n", rthread3_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread3_name);
		goto r_thread8;
	}
	rthread4 = kthread_run(rthread_func, rthread4_name, rthread4_name);
	if (rthread4)
		pr_info("Thread %s created!\n", rthread4_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread4_name);
		goto r_thread9;
	}
	rthread5 = kthread_run(rthread_func, rthread5_name, rthread5_name);
	if (rthread5)
		pr_info("Thread %s created!\n", rthread5_name);
	else
	{
		pr_err("Cannot create thread %s\n", rthread5_name);
		goto r_thread10;
	}

	return 0;

r_thread10:
	kthread_stop(rthread5);
r_thread5:
	kthread_stop(wthread5);
r_thread9:
	kthread_stop(rthread5);
r_thread4:
	kthread_stop(wthread4);
r_thread8:
	kthread_stop(rthread4);
r_thread3:
	kthread_stop(wthread3);
r_thread7:
	kthread_stop(rthread3);
r_thread2:
	kthread_stop(wthread2);
r_thread6:
	kthread_stop(rthread2);
r_thread1:
	return -1;
}

static void __exit my_mod_exit(void)
{
	kthread_stop(wthread5);
	kthread_stop(wthread4);
	kthread_stop(wthread3);
	kthread_stop(wthread2);
	kthread_stop(wthread1);
	kthread_stop(rthread5);
	kthread_stop(rthread4);
	kthread_stop(rthread3);
	kthread_stop(rthread2);
	kthread_stop(rthread1);
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo seqlock!");

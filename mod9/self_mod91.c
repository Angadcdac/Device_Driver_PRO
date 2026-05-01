/* Kernel linked list - static nodes */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

// linked list node definition
struct my_node
{
	int data;
	struct list_head my_list;
};

// create list head pointer/node
LIST_HEAD(my_head);

static int __init my_mod_init(void)
{
	int count = 0;
	struct my_node *tmp;

	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	
	// create node1 using option #1
	struct my_node node1 =  
	{
		.data=10,
		.my_list = LIST_HEAD_INIT(node1.my_list)
	};
	list_add(&node1.my_list, &my_head);

	struct my_node node2;
	node2.data = 20;
	INIT_LIST_HEAD(&node2.my_list);
	list_add(&node2.my_list, &my_head);

	struct my_node node3;
	node3.data = 30;
	INIT_LIST_HEAD(&node3.my_list);
	list_add(&node3.my_list, &my_head);

	struct my_node node4;
	node4.data = 40;
	INIT_LIST_HEAD(&node4.my_list);
	list_add(&node4.my_list, &my_head);

	struct my_node node5;
	node5.data = 50;
	INIT_LIST_HEAD(&node5.my_list);
	list_add(&node5.my_list, &my_head);

	struct my_node node6;
	node6.data = 60;
	INIT_LIST_HEAD(&node6.my_list);
	list_add(&node6.my_list, &my_head);

	struct my_node node7;
	node7.data = 70;
	INIT_LIST_HEAD(&node7.my_list);
	list_add(&node7.my_list, &my_head);

	struct my_node node8;
	node8.data = 80;
	INIT_LIST_HEAD(&node8.my_list);
	list_add(&node8.my_list, &my_head);
	
	list_for_each_entry(tmp, &my_head, my_list)
	{
		pr_info("Node %d, data=%d\n", count++, tmp->data);
	}

	return 0;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Kernel linked list - static nodes!");

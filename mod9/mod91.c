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
	
	// create node1 using option and assigning the variable#1
	struct my_node node1 =  
	{
		.data=10,
		.my_list = LIST_HEAD_INIT(node1.my_list)
	};
	// add node1
	list_add(&node1.my_list, &my_head);

	// create node2 using option and assigning the variable #2
	struct my_node node2;
	node2.data = 20;
	INIT_LIST_HEAD(&node2.my_list);
	// add node2
	list_add(&node2.my_list, &my_head);
	
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
/*
// linked list node definition
struct my_node
{
        int data;                      // data stored in node
        struct list_head my_list;     // kernel linked list structure
};

// create and initialize list head (empty list)
LIST_HEAD(my_head);
// Expands to: struct list_head my_head = { &my_head, &my_head };
*/
/*********************************************************************************************************************/
/*
 * Here is two type of initialization 1 is static initializaion and second is dynamic initialization We can add it using
 * 3 method availble
static int __init my_mod_init(void)
{
        int count = 0;
        struct my_node *tmp;

        pr_info("Hello world from %s!\n", KBUILD_MODNAME);

        // -------------------------------
        // Node1 (Static Initialization)
        // -------------------------------
        struct my_node node1 =
        {
                .data = 10,
                .my_list = LIST_HEAD_INIT(node1.my_list)
                // Initializes node1.my_list → points to itself
        };

        list_add(&node1.my_list, &my_head);
        // Adds node1 at beginning of list

        // -------------------------------
        // Node2 (Dynamic Initialization)
        // -------------------------------
        struct my_node node2;

        node2.data = 20;

        INIT_LIST_HEAD(&node2.my_list);
        // Initializes node2.my_list at runtime

        list_add(&node2.my_list, &my_head);
        // Adds node2 at beginning → becomes new head element

        // -------------------------------
        // Traversal
        // -------------------------------
        list_for_each_entry(tmp, &my_head, my_list)
        {
                pr_info("Node %d, data=%d\n", count++, tmp->data);
                // Iterates through all nodes
        }

        return 0;
}
*/

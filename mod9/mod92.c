/* Kernel linked list - dynamic nodes */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

// number of nodes in the kernel linked list
#define NUM_NODES	(10)

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
	int count, data;
	struct my_node *node, *tmp;

	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	
	data = 10;
	// create nodes dynamically via option #3
	// Add at the first 
	for (count=0;count<NUM_NODES;count++)
	{
		// kmalloc node
		node = (struct my_node *)kmalloc(sizeof(struct my_node), GFP_KERNEL);
		node->data = data;
		// enable its 'linked list state'
		INIT_LIST_HEAD(&node->my_list);
		// add node
		list_add(&node->my_list, &my_head);
		data += 10;
	}
	
	// forward traversal
	pr_info("Linked list forwards:\n");
	count  = 0;
	list_for_each_entry(tmp, &my_head, my_list)
	{
		pr_info("Node %d, data=%d\n", count++, tmp->data);
	}

	// reverse traversal
	pr_info("Linked list backwards:\n");
	count  = 0;
	list_for_each_entry_reverse(tmp, &my_head, my_list)
	{
		pr_info("Node %d, data=%d\n", count++, tmp->data);
	}

	return 0;
}

static void __exit my_mod_exit(void)
{
	struct my_node *node, *tmp;

	list_for_each_entry_safe(node, tmp, &my_head, my_list)
	{	// delete the node and free its instantiations
		pr_info("Deleting node with data=%d\n", node->data);
		list_del(&node->my_list);
		kfree(node);
	}
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Kernel linked list - dynamic nodes!");
/**
 * 
 * Create NUM_NODES linked list nodes dynamically.
 * Each iteration allocates a new node, initializes it,
 * and inserts it into the linked list.
 */
//for (count = 0; count < NUM_NODES; count++)
//{
        /*
         * Allocate memory for a new node in kernel space.
         * GFP_KERNEL → normal allocation (can sleep).
         */
//        node = (struct my_node *)kmalloc(sizeof(struct my_node), GFP_KERNEL);

        /*
         * Assign data to the node.
         * Data starts from initial value and increments by 10 each time.
         */
//         node->data = data;

        /*
         * Initialize the list_head inside the node.
         * This prepares the node to be linked into the list
         * by setting next and prev pointers to itself.
         */
//       INIT_LIST_HEAD(&node->my_list);

        /*
         * Insert the node into the linked list.
         * list_add() adds the node right after the head,
         * resulting in LIFO (stack-like) behavior.
         */
//     list_add(&node->my_list, &my_head);

        /*
         * Update data for the next node.
         */
 //       data += 10;
//}
 

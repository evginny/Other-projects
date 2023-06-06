#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

/* System call stub for initialize_bar() */
int(*STUB_initialize_bar)(void) =  NULL;
EXPORT_SYMBOL(STUB_initialize_bar);

/* System call stub for customer_arrival_bar(int, int) */
int(*STUB_customer_arrival)(int, int) = NULL;
EXPORT_SYMBOL(STUB_customer_arrival);


/* System call stub for close_bar() */
int(*STUB_close_bar)(void) = NULL;
EXPORT_SYMBOL(STUB_close_bar);


/*init bar wrapper*/
SYSCALL_DEFINE0(initialize_bar){
	printk(KERN_NOTICE "Inside initialize_bar SYCALL_DEFINE0 block");
	if(STUB_initialize_bar != NULL)
		return STUB_initialize_bar();
	else
		return -ENOSYS;
}

 /*close  bar wrapper*/
SYSCALL_DEFINE0(close_bar){
	printk(KERN_NOTICE "Inside close_bar SYCALL_DEFINE0 block");
	if(STUB_close_bar != NULL)
		return STUB_close_bar();
	else
		return -ENOSYS;
}

/*customer arrival  wrapper*/
SYSCALL_DEFINE2(customer_arrival, int, number_of_customers, int, type){
	printk(KERN_NOTICE "Inside customer_arrival SYCALL_DEFINE2 block");
	if(STUB_customer_arrival != NULL)
		return STUB_customer_arrival(number_of_customers, type);
	else
		return -ENOSYS;
}


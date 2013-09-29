#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>

#define SYSCALL_REPLACE 352

typedef unsigned long addr_t;

addr_t ** systable = NULL;
addr_t * replaced = NULL;

//Fake Syscall
long my_loaded_syscall(void * params)
{
	printk(KERN_INFO "Call loaded syscall");
	return 0;
}

//Systable
static addr_t ** get_systable_addr(void)
{
	//Start at begin of memory
	addr_t * begin = 0;
	addr_t search = (addr_t) &sys_close;

	while (1) {
		if (*begin == search) {
			//Found systable
			return (addr_t **) begin;
		}
		begin++;
	}
}

//Unlock write in read-only memory
static void unlock_write(void)
{
	write_cr0(read_cr0() | 0x10000);
}

//Lock write in read-only memory
static void  lock_write(void)
{
	write_cr0(read_cr0() & 0x10000);
}

//Init
static int __init my_module_init(void)
{
	printk(KERN_INFO "Load My Module ...");

	systable = get_systable_addr();

	unlock_write();
	replaced = systable[SYSCALL_REPLACE];
	systable[SYSCALL_REPLACE] = (addr_t *) &my_loaded_syscall;
	lock_write();

	printk(KERN_INFO "... done!");

	return 0;
}

//Exit
static void __exit my_module_exit(void)
{
	printk(KERN_INFO "Unload My Module ...");

	unlock_write();
	systable[SYSCALL_REPLACE] = replaced;
	lock_write();

	printk(KERN_INFO "... done!");
}

//Module
module_init(my_module_init);
module_exit(my_module_exit);

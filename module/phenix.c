#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

//Address
#if defined __X86_64__
#define __POINTER_TYPE 	uint64_t
#define BEGIN_ADDR 	((addr_t) 0xffffffff81000000)
#define END_ADDR 	((addr_t) 0xffffffffa2000000)
#define SPK 		"%lX"
#elif defined __i386__
#define __POINTER_TYPE 	uint32_t
#define BEGIN_ADDR	((addr_t) 0xc0000000)
#define END_ADDR	((addr_t) 0xd0000000)
#define SPK 		"%X"
#else
#error "Incorrect architecture"
#endif

typedef __POINTER_TYPE * addr_t;

#define __FIRST_SYSCALL_ADDR 	sys_close
#define __FIRST_SYSCALL_NUM 	__NR_close
#define INSERT_INDEX 352

static addr_t sys_saved = NULL;

/**
 *
 *
 */
asmlinkage long sys_dynamic_phenix(void * params)
{
	printk(KERN_INFO "Call my dynamic syscall with address : " SPK "\n", (__POINTER_TYPE) params);

	return 12345;
}

/**
 *
 *
 */
addr_t * find_systable(void)
{
	addr_t * table;
	addr_t i = BEGIN_ADDR;

	while (i < END_ADDR) {
		table = (addr_t *) i;
		if (table[__FIRST_SYSCALL_NUM] == (addr_t) __FIRST_SYSCALL_ADDR) {
			printk(KERN_INFO "Syscall table found at " SPK "\n", (__POINTER_TYPE) table);
			return table;
		}
		i++;
	}

	return NULL;
}

/**
 *
 *
 */
int my_module_init(void)
{
	addr_t * table;

	printk(KERN_INFO "Load My Module ...\n");

	table = find_systable();

	if (table == NULL) {
		return -1;
	}

	//Unlock write memory
	write_cr0(read_cr0() & (~ 0x10000));

	//Change syscall
	sys_saved = table[INSERT_INDEX];
	table[INSERT_INDEX] = (addr_t) sys_dynamic_phenix;

	//Lock write memory
	write_cr0(read_cr0() | 0x10000);

	printk(KERN_INFO "... done!\n");

	return 0;
}

/**
 *
 *
 */
void my_module_exit(void)
{
	addr_t * table;

	printk(KERN_INFO "Unload My Module ...\n");

	table = find_systable();

	if (table == NULL || sys_saved != NULL) {
		return;
	}

	//Unlock write memory
	write_cr0(read_cr0() & (~ 0x10000));

	//Replace syscall
	table[INSERT_INDEX] = sys_saved;

	//Lock write memory
	write_cr0(read_cr0() | 0x10000);

	printk(KERN_INFO "... done!\n");
}

//Module register
module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");

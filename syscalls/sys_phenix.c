#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage long sys_phenix(int i, void * params)
{
	if (i == 1337) {
		int * tmp = (int *) params;
		*tmp = 1337;
	}
	return 1337;
}

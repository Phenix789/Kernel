#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main() {
	long i = printf("%ld\n", syscall(352, NULL));
	printf("Syscall return : %lX\n", i);
	return 0;
}

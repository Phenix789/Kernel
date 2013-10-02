#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main() {
	printf("Syscall return %ld\n", syscall(352, NULL));
	return 0;
}

#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main() {
	printf("%ld\n", syscall(351, 0, -1));
	return 0;
}

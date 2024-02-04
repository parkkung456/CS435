#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
	for (int i = 0; i < 2; i++) {
		pid_t p;
		p = fork();
		if (p == 0)
			printf("Child : My PID is %d\t My PPID is %d\n", getpid(), getppid());
		else {
			printf("Parent : My PID is %d\n", getpid());
			wait(NULL);
		}
	}
	return 0;
}

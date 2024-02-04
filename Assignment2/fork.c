#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
	for (int i = 0; i < 4; i++) {
		pid_t p, c;
		p = fork();
		if (p == 0) {
			printf("Child : %d\n", getpid());
		}
		else {
			if (i == 0)	
				printf("Parent : %d\n",getpid());
			wait(NULL);	
		}
	}
	return 0;
}

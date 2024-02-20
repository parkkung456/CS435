#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(void) {
	int fd[2], fd2[2];
	char line[20] = "ping", line2[20] = "pong", buf[20];
	pid_t pid;
	int c;

	if (pipe(fd) < 0) //first pipe
		perror("pipe error");
	if (pipe(fd2) < 0) //second pipe
		perror("pipe error");

	//child1
	if ((pid = fork()) < 0)
		perror("fork error");
	else if (pid == 0) {
		close(fd[0]); //child1 is not reading from first pipe
		close(fd2[1]); //child1 is not writing in second pipe
		write(fd[1], line, 20); //child1 is writing in first pipe
		close(fd[1]); //child1 quit writing in first pipe
		read(fd2[0], buf, 20); //child1 is reading from second pipe
		close(fd2[0]); //child1 quit reading from second pipe
		buf[c] = '\0';
		printf("%s\n", line2);
		exit(0);
		
	}
	
	//child2
	if ((pid = fork()) < 0)
		perror("fork error");
	else if (pid == 0) {
		close(fd2[0]); //child2 is not reading from second pipe
		close(fd[1]); //child2 is not writing in first pipe
		write(fd2[1], line2, 20); //child2 is writing in second pipe
		close(fd2[1]); //child2 quit writing in second pipe
		read(fd[0], buf, 20); //child2 is reading from first pipe
		close(fd[0]); //child2 quit reading from first pipe
		buf[c] = '\0';
		printf("%s\n", line);
	}
	
	wait(NULL);
	exit(0);
}

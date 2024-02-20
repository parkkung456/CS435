#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(void) {
	int fd[2];
	char buf[20], line[20] = "hello from parent\n";
	pid_t pid;
	int c;

	if (pipe(fd) < 0)
		perror("pipe error");
	if ((pid = fork()) < 0)
		perror("fork error");
	else if (pid > 0) {
		close(fd[0]); //parent is not reading from pipe
		write(fd[1], line, 20); //parent is writing in pipe
	}
	else {
		close(fd[1]); //child is not writing in pipe
		c = read(fd[0], line, 20); //child is reading from pipe
		buf[c] = '\0';
		printf("%s\n", line);
	}
	exit(0);
}

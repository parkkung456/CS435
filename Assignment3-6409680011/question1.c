#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main(void) {
    pid_t pid;
    if ((pid = fork()) < 0) {
	    perror("fork error");
    }
    else if (pid == 0) {
	    chdir("/home/kali"); //assign directory
	    if (execl("/home/kali/cs435/asm3/echoall", "echoall", "myarg1", "MY ARG2", NULL) < 0) //exec run echoall.c program with specified arguments
		    perror("execle error"); //only run this line when execl error
    }

    if ((waitpid(pid, NULL, 0)) < 0) //told parents to wait for there child
	    perror("wait error");
    
    if ((pid = fork()) < 0) //fork again
    	perror("fork error");
    else if (pid == 0) { //if fork process succeeds, then run echoall.c program with specified arguments
    	if (execl("/home/kali/cs435/asm3/echoall", "echoall", "only 1 arg", NULL) < 0)
		perror("execlp error");
    
    }

 return 0;
}

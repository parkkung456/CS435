#include "apue.h"

int main(void) {
    int n;
    int pipe1[2], pipe2[2]; // Define two pipes
    pid_t pid1, pid2;
    char line[MAXLINE];
    
    if ((pipe(pipe1)) < 0 || (pipe(pipe2)) < 0) // Create two pipes
        err_sys("pipe error");
        
    if ((pid1 = fork()) < 0) {
        err_sys("fork error");
    } else if (pid1 > 0) { // Parent process
        close(pipe1[0]); // Close reading end of pipe1
        close(pipe2[0]); // Close reading end of pipe2
        close(pipe2[1]); // Close writing end of pipe2
        
        write(pipe1[1], "hello from parent\n", 18); // Write to pipe1
        
        waitpid(pid1, NULL, 0); // Wait for child1
        
    } else { // Child1 process
        if ((pid2 = fork()) < 0) {
            err_sys("fork error");
        } else if (pid2 == 0) { // Child1 process
            close(pipe1[1]); // Close writing end of pipe1
            close(pipe1[0]); // Close reading end of pipe1
            
            // Duplicate pipe2[1] to stdout
            if (dup2(pipe2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");
            
            // Close unused pipe descriptors
            close(pipe2[0]);
            close(pipe2[1]);
            
            // Execute /bin/cat
            execl("/bin/cat", "cat", (char *)0);
            err_sys("execl error");
            
        } else { // Child2 process
            close(pipe1[0]); // Close reading end of pipe1
            close(pipe1[1]); // Close writing end of pipe1
            close(pipe2[1]); // Close writing end of pipe2
            
            n = read(pipe2[0], line, MAXLINE); // Read from pipe2
            if (n <= 0) // Check for error or end of file
                err_sys("read error or end of file");
            
            printf("Child2 received: %s", line); // Print received message
        }
    }
    exit(0);
}


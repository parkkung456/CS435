#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close the write end of the pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read from the pipe
        close(pipefd[0]); // Close the read end of the pipe

        execlp("wc", "wc", "-l", NULL); // Execute wc -l command
        perror("exec");
        exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            if (write(pipefd[1], buffer, bytes_read) != bytes_read) {
                perror("write to pipe");
                break;
            }
        }

        close(pipefd[1]); // Close the write end of the pipe
        fclose(file);

        // Wait for the child process to finish
        wait(NULL);
    }

    return EXIT_SUCCESS;
}


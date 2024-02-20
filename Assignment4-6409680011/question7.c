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

    if (pid == 0) { // Child Process
        close(pipefd[1]); // ปิด pipe ที่เขียน
        dup2(pipefd[0], STDIN_FILENO); // เปลี่ยน stdin เพื่ออ่านจาก pipe
        close(pipefd[0]); // ปิด pipe ที่อ่าน

        execlp("wc", "wc", "-l", NULL); // ให้ใช้คำสั่ง wc -l
        perror("exec");
        exit(EXIT_FAILURE);
    } else { // Parent Process
        close(pipefd[0]); // ปิด pipe ที่อ่าน

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            if (write(pipefd[1], buffer, bytes_read) != bytes_read) {
                perror("write to pipe");
                break;
            }
        }

        close(pipefd[1]); // ปิดส่วน pipe ที่เขียน
        fclose(file);

        // รอ Child Process จบการทำงาน
        wait(NULL);
    }

    return EXIT_SUCCESS;
}

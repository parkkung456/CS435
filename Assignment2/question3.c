#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    // สร้าง child process ด้วย fork()
    pid = fork();

    // ตรวจสอบว่าเป็น parent process หรือ child process
    if (pid > 0) {
        // parent process
        printf("I am parent\n");
        // รอ child process จบการทำงาน ด้วย waitpid()
        waitpid(pid, &status, 0);
        printf("finish\n");
    } else if (pid == 0) {
        // child process
        printf("I am a child\n");
        // ทำงานของ child process ตรงนี้
    } else {
        // เกิดข้อผิดพลาดในการสร้าง child process
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    // สร้างเพียง 1 child process ด้วย fork()
    pid = fork();

    // ตรวจสอบว่าเป็น parent process หรือ child process
    if (pid == 0) {
        // child process
        printf("I am child\n");

        // เปลี่ยนไดเรกทอรีทำงานใน child process
        chdir("/home/iampark");

        // ใช้ execlp เพื่อรันคำสั่ง ls
        execlp("ls", "ls", "-l", "-r", "-g", "-k", "-i", "-t", "-R", (char *)NULL);

        // กรณี execlp ไม่สำเร็จ
        perror("execlp");
        exit(1);
    } else if (pid < 0) {
        // เกิดข้อผิดพลาดในการสร้าง child process
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    // parent process
    printf("I am parent\n");

    // รอ child process จบการทำงาน ด้วย waitpid()
    waitpid(pid, &status, 0);

    printf("finish\n");

    return 0;
}


#include "apue.h"
#include <sys/wait.h>
int main(void) {
    int n;
    int pipe1[2], pipe2[2]; // กำหนด pipe สองตัว
    pid_t pid1, pid2;
    char line[MAXLINE];
    
    if ((pipe(pipe1)) < 0 || (pipe(pipe2)) < 0) // สร้าง pipe สองตัว
        err_sys("pipe error");
        
    if ((pid1 = fork()) < 0) {
        err_sys("fork error");
    } else if (pid1 > 0) { // กระบวนการหลัก
        close(pipe1[0]); // ปิดที่อ่านของ pipe 1
        close(pipe2[0]); // ปิดที่อ่านของ pipe 2
        close(pipe2[1]); // ปิดที่เขียนของ pipe 2
        
        write(pipe1[1], "hello from parent\n", 18); // เขียนไปที่ pipe 1
        
        waitpid(pid1, NULL, 0); // รอกระบวนการลูก
        
    } else { // กระบวนการลูก 1
        if ((pid2 = fork()) < 0) {
            err_sys("fork error");
        } else if (pid2 > 0) { // กระบวนการลูก 1
            close(pipe1[1]); // ปิดที่เขียนของ pipe 1
            close(pipe2[0]); // ปิดที่อ่านของ pipe 2
            
            n = read(pipe1[0], line, MAXLINE); // อ่านจาก pipe 1
            if (n <= 0) 
                err_sys("read error");
            
            printf("Child1 received: %s", line); // พิมพ์ข้อความ
            
            write(pipe2[1], line, n); // เขียนไปmuj pipe 2
            
            waitpid(pid2, NULL, 0); // รอ Child Process 2
        } else { // กระบวนการลูก 2
            close(pipe1[0]); // ปิดที่อ่านของ pipe 1
            close(pipe1[1]); // ปิดที่เขียนของ pipe 1
            close(pipe2[1]); // ปิดที่เขียนของ pipe 2
            
            n = read(pipe2[0], line, MAXLINE); // อ่านจาก pipe 2
            if (n <= 0) 
                err_sys("read error or end of file");
            
            printf("Child2 received: %s", line); // พิมพ์ข้อความที่ได้รับ
        }
    }
    exit(0);
}

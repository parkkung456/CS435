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
        close(pipe1[0]); // ปิดส่วนที่อ่านของ pipe 1
        close(pipe2[0]); // ปิดส่วนที่อ่านของ pipe 2
        close(pipe2[1]); // ปิดส่วนที่เขียนของ pipe 2
        
        write(pipe1[1], "hello from parent\n", 18); // ส่งไปยัง pipe 1
        
        waitpid(pid1, NULL, 0); // รอสำหรับกระบวนการลูก 1
        
    } else { // Child Process 1
        if ((pid2 = fork()) < 0) {
            err_sys("fork error");
        } else if (pid2 == 0) { // Child Process 1
            close(pipe1[1]); // ปิดส่วนที่เขียนของ pipe 1
            close(pipe1[0]); // ปิดส่วนที่อ่านของ pipe 1
            
            // duplicate pipe2[1] ไปยัง stdout
            if (dup2(pipe2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");
            
            // ปิด pipe ที่ไม่ได้ใช้
            close(pipe2[0]);
            close(pipe2[1]);
            
            // รัน /bin/cat
            execl("/bin/cat", "cat", (char *)0);
            err_sys("execl error");
            
        } else { // Child Process 2
            close(pipe1[0]); // ปิดส่วนที่อ่านของ pipe 1
            close(pipe1[1]); // ปิดส่วนที่เขียนของ pipe 1
            close(pipe2[1]); // ปิดส่วนที่เขียนของ pipe 2
            
            n = read(pipe2[0], line, MAXLINE); // อ่านจาก pipe 2
            if (n <= 0) 
                err_sys("read error or end of file");
            
            printf("Child2 received: %s", line); // พิมพ์ข้อความที่ได้รับ
        }
    }
    exit(0);
}

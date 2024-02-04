#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print_pid_info(int gen) {
    // พิมพ์ข้อมูล PID ของโปรเซสปัจจุบันและ PID ของโปรเซสแม่
    printf("Generation %d - My PID is %d\n", gen, getpid());
    printf("Generation %d - My Parent PID is %d\n", gen, getppid());
}

void process_generation(int gen, int max_gen) {
    if (gen <= max_gen) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            // เรียกใช้ฟังก์ชันเพื่อสร้าง child process ของ generation ถัดไป
            process_generation(gen + 1, max_gen);
            // ออกจาก child process
            exit(0);
        } else if (pid > 0) {
            // Parent process
            // รอให้ child process ทำงานเสร็จและจบการทำงาน
            waitpid(pid, NULL, 0);
            // พิมพ์ข้อมูล PID ของโปรเซสปัจจุบันและ PID ของโปรเซสลูก
            print_pid_info(gen);
        } else {
            // กรณีที่ fork() ไม่สำเร็จ
            perror("fork() failed");
            // ออกจากโปรแกรมด้วยการระบุสถานะ EXIT_FAILURE
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    // กำหนด generation สูงสุดเป็น 5
    int max_generation = 5;
    // เรียกใช้ฟังก์ชัน process_generation เพื่อเริ่มกระบวนการสร้างโปรเซส
    process_generation(1, max_generation);

    return 0;
}

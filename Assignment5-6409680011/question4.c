#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 40 // กำหนดค่าของ n

// ฟังก์ชั่นสำหรับคำนวณ dot product ของส่วนของ vector
void dot_product(int *x, int *y, int start, int end, int pipe_write) {
    int result = 0;
    for (int i = start; i <= end; i++) {
        result += x[i] * y[i];
    }
    // ส่งผลลัพธ์ไปยัง parent process ผ่าน pipe
    write(pipe_write, &result, sizeof(int));
}

int main() {
    int x[N], y[N];
    int pfd[4][2]; // เก็บค่า file descriptors ของ pipes 4 อัน

    // กำหนดค่าให้ vector x และ y
    for (int i = 0; i < N; i++) {
        x[i] = i + 1;
        y[i] = i + 1;
    }

    // สร้าง pipes 4 อัน
    for (int i = 0; i < 4; i++) {
        if (pipe(pfd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // สร้าง child process 4 โปรเซส
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            int start = i * 10;
            int end = start + 9;
            close(pfd[i][0]); // ปิดการอ่าน pipe ที่ไม่จำเป็น
            dot_product(x, y, start, end, pfd[i][1]);
            close(pfd[i][1]); // ปิดการเขียน pipe หลังจากใช้งานเสร็จ
            exit(0);
        } else if (pid < 0) {
            fprintf(stderr, "Error in forking process\n");
            return 1;
        }
    }

    // รอให้ child process ทำงานเสร็จ
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    // อ่านค่าจาก pipes และแสดงผลลัพธ์ทั้งหมด
    int result, total_result = 0;
    for (int i = 0; i < 4; i++) {
        close(pfd[i][1]); // ปิดการเขียน pipe ที่ไม่จำเป็น
        while (read(pfd[i][0], &result, sizeof(int)) > 0) {
            total_result += result;
        }
        close(pfd[i][0]); // ปิดการอ่าน pipe หลังจากใช้งานเสร็จ
    }

    printf("Total dot product: %d\n", total_result);

    return 0;
}


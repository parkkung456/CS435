#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct strline {
    char *line;
    int strsize;
    int linesize;
} strline;

int main() {
    // สร้าง strline ขนาด 5 elements
    strline *lines = calloc(5, sizeof(strline));

    // กำหนดค่า linesize เป็น 10 และ strsize เป็น 0 ใน element แรก
    lines[0].linesize = 10;
    lines[0].strsize = 0;

    // จองพื้นที่สำหรับ string ขนาด 10 bytes ใน element แรก
    lines[0].line = malloc(10 * sizeof(char));

    // สร้าง buffer ขนาด 100 bytes
    char buffer[100];

    int line_count = 0; // นับจำนวนบรรทัด

    // รับข้อมูลจาก stdin และประมวลผลตามเงื่อนไข
    while (fgets(buffer, sizeof(buffer), stdin) && line_count < 20) {
        // คัดลอกข้อมูลบรรทัดแรกมาใน line ของ strline แรก
        if (strlen(buffer) <= 10) {
            strcpy(lines[line_count].line, buffer);
            lines[line_count].strsize = strlen(buffer);
        } else if (strlen(buffer) > lines[line_count].linesize) {
            // ปรับขนาด linesize เป็นหลักสิบที่พอเพียง
            lines[line_count].linesize = ((strlen(buffer) / 10) + 1) * 10;
            lines[line_count].line = realloc(lines[line_count].line, lines[line_count].linesize * sizeof(char));
            strcpy(lines[line_count].line, buffer);
            lines[line_count].strsize = strlen(buffer);
        }

        // เมื่อมีการกด Enter ให้เลื่อนไปใช้ element ถัดไปของ strline และจองพื้นที่ใหม่สำหรับ line
        if (buffer[strlen(buffer) - 1] == '\n') {
            line_count++;
            if (line_count < 5) {
                lines[line_count].line = malloc(10 * sizeof(char));
                lines[line_count].linesize = 10;
                lines[line_count].strsize = 0;
            } else if (line_count % 5 == 0) {
                // เพิ่มขนาด strline อีก 5 elements และปรับขนาด linesize ตามที่กำหนด
                lines = realloc(lines, (line_count + 5) * sizeof(strline));
                for (int i = line_count; i < line_count + 5; i++) {
                    lines[i].line = malloc(10 * sizeof(char));
                    lines[i].linesize = 10;
                    lines[i].strsize = 0;
                }
            }
        }
    }

    // พิมพ์ข้อมูลทุกบรรทัดออกทาง stdout
    for (int i = 0; i < line_count; i++) {
        printf("%s", lines[i].line);
    }

    // คืนหน่วยความจำ
    for (int i = 0; i < line_count; i++) {
        free(lines[i].line);
    }
    free(lines);

    return 0;
}


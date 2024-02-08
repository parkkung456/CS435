#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct strline {
    char *line;
    int strsize; 
    int linesize;
} strline;

int main() {
    strline *lines = calloc(5, sizeof(strline)); // สร้าง strline aขนาด 5 elements ตามภาพ
    if (lines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // กำหนด element แรก
    lines[0].linesize = 10;
    lines[0].strsize = 0;
    lines[0].line = (char *)malloc(lines[0].linesize * sizeof(char));
    printf("linesize for line[1]: %d\n", lines[0].linesize);
    // ตรวจสอบการ allocate memory 
    if (lines[0].line == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // สร้าง elements ถัดไปโดยใช้ลูป
    for (int next_index = 1; next_index < 5; next_index++) {
	int next_size = lines[next_index - 1].linesize + 10;
        lines[next_index].linesize = next_size; // กำหนดค่า linesize
        lines[next_index].strsize = 0; // กำหนดค่า strsize
        lines[next_index].line = (char *)malloc(next_size * sizeof(char));
	printf("linesize for line[%d]: %d\n", next_index + 1, lines[next_index].linesize);

        // ตรวจสอบการ allocate memory 
        if (lines[next_index].line == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }
    }

    // รับข้อมูลจากผู้ใช้และแสดงผล
    for (int next_index = 0; next_index < 5; next_index++) {
        printf("Enter a line: ");
        int next_size = lines[next_index].linesize;
        fgets(lines[next_index].line, next_size, stdin);
    }
     for (int next_index = 0; next_index < 5; next_index++) {
        printf("You entered: %s", lines[next_index].line);
    }

    // free resource 
    for (int i = 0; i < 5; i++) {
        free(lines[i].line);
    }
    free(lines);

    return 0;
}


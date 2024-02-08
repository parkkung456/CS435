#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // ข้อ a
    // ใช้ malloc() เพื่อจองพื้นที่ขนาด 50 bytes และใช้ memset() เพื่อกำหนดค่า 'a' ให้กับทุก character
    char *ptr_a = (char *)malloc(50 * sizeof(char));
    if (ptr_a == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    memset(ptr_a, 'a', 50);

    // ข้อ b
    // ใช้ calloc() เพื่อจองพื้นที่ขนาด 50 bytes และตรวจสอบว่าค่าเริ่มต้นเป็น 0 จริงหรือไม่
    char *ptr_b = (char *)calloc(50, sizeof(char));
    if (ptr_b == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    for (int i = 0; i < 50; i++) {
        if (ptr_b[i] != 0) {
            printf("calloc() didn't initialize memory to zero.\n");
            break;
        }
    }

    // ข้อ c
    // ใช้ realloc() เพื่อขยายขนาดพื้นที่จาก 50 bytes เป็น 75 bytes
    char *nptr = realloc(ptr_a, 75 * sizeof(char));
    if (nptr == NULL) {
        printf("Memory reallocation failed!\n");
        free(ptr_a);
        return 1;
    }

    printf("nptr address: %p\n", (void *)nptr);
    printf("ptr_a address: %p\n", (void *)ptr_a);


    // คืนทรัพยากร
    free(nptr);
    free(ptr_b);

    return 0;
}


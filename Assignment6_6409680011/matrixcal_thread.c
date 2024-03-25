#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ประกาศโครงสร้างข้อมูลสำหรับส่งพารามิเตอร์ให้กับThread
struct ThreadArgs {
    double **A; // เก็บเมทริกซ์ A
    double **B; // เก็บเมทริกซ์ B
    double **C; // เก็บเมทริกซ์ C
    double **D; // เก็บเมทริกซ์ D
    int startRow; // แถวเริ่มต้นที่ Thread จะคำนวณ
    int endRow; // แถวสุดท้ายที่Threadจะคำนวณ
    int N; // ขนาดของเมทริกซ์
    double *threadTotal; // ผลรวมของแต่ละThread
    pthread_mutex_t *mutex; // Mutex สำหรับการป้องกันการแชร์ข้อมูล
};

// ฟังก์ชันสำหรับกำหนดค่าเริ่มต้นให้กับเมทริกซ์ A, B, C, D
void initializeMatrices(double **A, double **B, double **C, double **D, int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = 4.0;
            B[i][j] = 3.0;
            C[i][j] = 8.0;
            D[i][j] = 0.0;
        }
    }
}

// ฟังก์ชันสำหรับการคำนวณเมทริกซ์
void *matrixCalculation(void *args) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;
    double **A = threadArgs->A;
    double **B = threadArgs->B;
    double **C = threadArgs->C;
    double **D = threadArgs->D;
    int startRow = threadArgs->startRow;
    int endRow = threadArgs->endRow;
    int N = threadArgs->N;
    double *threadTotal = threadArgs->threadTotal;
    pthread_mutex_t *mutex = threadArgs->mutex; // รับ Mutex จากพารามิเตอร์

    double td = 0.0;

    int i, j, k;
    for (i = startRow; i <= endRow; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
            D[i][j] += C[i][j];
            td += D[i][j];
        }
    }

    // ล็อก Mutex เพื่อป้องกันการเข้าถึงตัวแปรที่ใช้ร่วมกัน
    pthread_mutex_lock(mutex);
    threadTotal[startRow] = td;
    pthread_mutex_unlock(mutex); // ปลดล็อก Mutex

    printf("Thread finished. Total sum for rows %d to %d: %f\n", startRow, endRow, td);

    pthread_exit(NULL);
}

// ฟังก์ชันหลักของโปรแกรม
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return -1;
    }

    int N = atoi(argv[1]);

    // จองพื้นที่หน่วยความจำสำหรับเมทริกซ์ A, B, C, D
    double **A = (double **)malloc(N * sizeof(double *));
    double **B = (double **)malloc(N * sizeof(double *));
    double **C = (double **)malloc(N * sizeof(double *));
    double **D = (double **)malloc(N * sizeof(double *));
    int i;
    for (i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
        B[i] = (double *)malloc(N * sizeof(double));
        C[i] = (double *)malloc(N * sizeof(double));
        D[i] = (double *)malloc(N * sizeof(double));
        if (A[i] == NULL || B[i] == NULL || C[i] == NULL || D[i] == NULL) {
            printf("Memory allocation error.\n");
            return -1;
        }
    }

    // กำหนดค่าเริ่มต้นให้กับเมทริกซ์ A, B, C, D
    initializeMatrices(A, B, C, D, N);

    // จองพื้นที่หน่วยความจำสำหรับผลรวมของแต่ละThread
    double *threadTotal = (double *)malloc(N * sizeof(double));
    if (threadTotal == NULL) {
        printf("Memory allocation error.\n");
        return -1;
    }
    pthread_t T1, T2, T3, T4; // ประกาศตัวแปรสำหรับเก็บ ID thread

    // สร้างโครงสร้างข้อมูลสำหรับส่งไปยัง thread แต่ละสายจะได้รับเมทริกซ์ที่แบ่งแยกเป็นส่วนๆ
    struct ThreadArgs args1 = {A, B, C, D, 0, N/4 - 1, N, threadTotal}; // ส่วนแรกของเมทริกซ์
    struct ThreadArgs args2 = {A, B, C, D, N/4, N/2 - 1, N, threadTotal}; // ส่วนที่สองของเมทริกซ์
    struct ThreadArgs args3 = {A, B, C, D, N/2, 3*N/4 - 1, N, threadTotal}; // ส่วนที่สามของเมทริกซ์
    struct ThreadArgs args4 = {A, B, C, D, 3*N/4, N - 1, N, threadTotal}; // ส่วนที่สี่ของเมทริกซ์

    // สร้าง Mutex
    pthread_mutex_t mutex;
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Mutex initialization failed.\n");
        return -1;
    }

    // กำหนด Mutex
    args1.mutex = &mutex;
    args2.mutex = &mutex;
    args3.mutex = &mutex;
    args4.mutex = &mutex;

    // สร้างการทำงานแต่ละสาย
    pthread_create(&T1, NULL, matrixCalculation, (void *)&args1);
    pthread_create(&T2, NULL, matrixCalculation, (void *)&args2);
    pthread_create(&T3, NULL, matrixCalculation, (void *)&args3);
    pthread_create(&T4, NULL, matrixCalculation, (void *)&args4);

    // รอให้ทำงานเสร็จ
    pthread_join(T1, NULL);
    pthread_join(T2, NULL);
    pthread_join(T3, NULL);
    pthread_join(T4, NULL);

    double totalSum = 0.0;
    for (i = 0; i < N; i++) {
        totalSum += threadTotal[i];
    }

    printf("Total sum of matrix D: %f\n", totalSum);

    // คืนหน่วยความจำที่ใช้
    for (i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
        free(D[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);
    free(threadTotal);

    // ลบ Mutex หลังจากใช้เสร็จ
    pthread_mutex_destroy(&mutex);

    return 0;
}
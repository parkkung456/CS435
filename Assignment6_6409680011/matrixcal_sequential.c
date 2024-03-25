#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	double **a, **b, **c, **d;
	int i, j, k, n = atoi(argv[1]);
	
	//allocate memory to every row in array a, b, c, d
	a = malloc(n * sizeof(double*));
	b = malloc(n * sizeof(double*));
	c = malloc(n * sizeof(double*));
	d = malloc(n * sizeof(double*));

	if (a == NULL || b == NULL || c == NULL || d == NULL) {
		printf("Memory allocation failed");
		return -1;
	}

	//allocate memory to every column in array a, b, c, d
	for (i = 0; i < n; i++) {
		a[i] = malloc(n * sizeof(double));
		b[i] = malloc(n * sizeof(double));
		c[i] = malloc(n * sizeof(double));
		d[i] = malloc(n * sizeof(double));
		if (a[i] == NULL || b[i] == NULL || c[i] == NULL || d[i] == NULL) {
		printf("Memory allocation failed");
		return -1;
		}
	}

	//insert double value to every element in array a, b, c, d
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			a[i][j] = 4.0;
			b[i][j] = 3.0;
			c[i][j] = 8.0;
			d[i][j] = 0.0;
		}
	}

	double td = 0.0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				//matrix multiplication a * b
				d[i][j] += a[i][k] * b[k][j];	
			}
			//matrix addition c
			d[i][j] += c[i][j];
			//result of addition of elements in matrix d
			td += d[i][j];
		}
	}
	
	printf("%.1f", td);
}

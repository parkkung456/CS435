#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc == 1) //if this program runs without any other than its name
		printf("\n No command line argument pass");
	if (argc ==2) // execute first argument
		system(argv[1]);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scrapper() { //scrapper function for removing file
	remove("commandstr.txt");
}

int main(int argc, char* argv[]) {
	if (argc == 1) //if this program runs without any other than its name
		printf("\n No command line argument pass");
	if (argc < 4 && argc > 1) { //if this program accepts less than 4 arguments but greater than 1 argument
		FILE *f;
		f = fopen("commandstr.txt", "w"); //create file only allowed writing
		fprintf(f, argv[1]); //write first argument in created file
		fclose(f);
		system(argv[1]); //execute first argument
		if (argv[2] == "0") //if it has exit code = 0, then exit the program without scrapper function
			exit(0);
		else if (argv[2] == "1" || argc == 2) { //if it has exit code = 1 or it has only one argument, then exit the program with scrapper funtion
			atexit(scrapper);
			scrapper();
		}
	}
	return 0;
}

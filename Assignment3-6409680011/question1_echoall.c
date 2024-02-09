#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
        int i;
        char **ptr;
        extern char **environ;
	
        for (i = 0; i < argc; i++) //print input argument when executed
                printf("argv[%d]: %s \n", i, argv[i]);

        for (ptr = environ; *ptr != 0; ptr++) { //print environment variables
                printf("%s \n", *ptr);
        }
        exit(0);
}

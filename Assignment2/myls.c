#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
int main(int argc, char *argv[])
{
	DIR				*dp;
	struct dirent	*dirp;
	char path[] = "..";
	if (argc != 2) {
		printf("usage: ls directory_name");
		return 1;
	}

	if ((dp = opendir(path)) == NULL) {
		printf("can't open %s", argv[1]);
		return 1;
	}
	while ((dirp = readdir(dp)) != NULL) {
		printf("%s\n", dirp->d_name);
	}

	closedir(dp);
	exit(0);
}

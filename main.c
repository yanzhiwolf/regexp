#include <stdio.h>
#include <stdlib.h>
#include "operator.h"
#include "re2post.h"


int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		printf("usage: re");
		return -1;
	}

	init();

	char *post = re2post(argv[1]);
	printf("%s\n", post);

	free(post);

	return 0;
}

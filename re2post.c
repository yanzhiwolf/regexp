#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "re2post.h"

static char *__format(const char *re)
{
	size_t len = strlen(re);
	char *formatted = (char*)malloc(len * 2);
	int pos = 0;

	for (size_t i = 0; i < len; i++) {
		int c1 = re[i];
		formatted[pos++] = c1;

		int c2;
		if (i+1 < len) {
			c2 = re[i+1];
			if (c1 == '(' || c1 == '^' || c1 == '|' || 
				c2 == ')' || c2 == '|' || c2 == '?' ||
				c2 == '+' || c2 == '*' || c2 == '^') {
				continue;
			}

			formatted[pos++] = '&';
		}
	}

	return formatted;
}

const char *re2post(const char *src)
{
	int top = 0;
	size_t len = strlen(src);
	char *stack = (char*)malloc(len);
	

	for (int i = 0; i < len; ++i)
	{
		
	}
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		printf("usage: re");
		return -1;
	}

	const char *formatted = __format(argv[1]);
	printf("%s\n", formatted);

	return 0;
}
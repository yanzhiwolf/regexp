#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "re2post.h"

static int op_attr[128] = {};

#define ATTRIBUTE(pri) pri
#define PRIORITY(c) op_attr[c]

static void __init()
{
	op_attr['|'] = ATTRIBUTE(1);
	op_attr['&'] = ATTRIBUTE(2);
	op_attr['*'] = ATTRIBUTE(3);
	op_attr['+'] = ATTRIBUTE(3);
	op_attr['?'] = ATTRIBUTE(3);
	op_attr['('] = ATTRIBUTE(4);
	op_attr[')'] = ATTRIBUTE(4);
}


char *format(const char *re)
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

char *re2post(const char *re)
{
	int top = 0;
	size_t len = strlen(re);

	stack<char> *s = new stack<char>(len);

	int post_pos = 0;
	char *post = (char*)malloc(len);
	
	for (int i = 0; i < len; ++i)
	{
		int c = re[len];
		if (!PRIORITY(c)) {
			post[post_pos++] = c;
			continue;
		}

		if (s->empty()) {
			s->push(c);
		}
		else {
			int top = s->top();
			int top_pri = PRIORITY(top);
			int c_pri = PRIORITY(c);
			while (top_pri >= c_pri) {
				if (top != '(') {
					post[post_pos++] = c;
				}

				s->pop();
				if (s->empty()) {
					break;
				}
				top = s->top();
				top_pri = PRIORITY(top);
			}
		}
	}

	while (!s->empty()) {
		post[post_pos++] = s->topAndPop();
	}

	return post;
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		printf("usage: re");
		return -1;
	}

	__init();

	char *formatted = format(argv[1]);
	printf("%s\n", formatted);

	char *post = re2post(formatted);
	printf("%s\n", post);

	free(formatted);
	free(post);

	return 0;
}
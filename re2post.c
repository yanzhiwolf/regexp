#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "re2post.h"

static unsigned char op_attr[128] = {};

/* n=Number of operands，
** a=associative
** p=priority
*/
enum AttrValue {
	EMPTY 		= 0,

	UNARY 		= 1,
	BINARY 		= 2,

	ASSO_LEFT 	= 1,
	ASSO_RIGHT 	= 2,

	PRI_1 		= 1,
	PRI_2 		= 2,
	PRI_3 		= 3,
	PRI_4 		= 4,
};

#define ATTRIBUTE(n,a,p) ((n<<6) | (a<<4) | (p<<0))

#define ISOP(c)     (op_attr[c] > 0)
#define OPNUM(c)	((op_attr[c] & 0xc0) >> 6)

#define ASSO(c)		((op_attr[c] & 0x30) >> 4)
#define ISASSOLEFT(c) 	(ASSO(c) == ASSO_LEFT)
#define ISASSORIGHT(c) 	(ASSO(c) == ASSO_RIGHT)

#define PRI(c) 		((op_attr[c] & 0x0f) >> 0)

static void __init()
{
	op_attr['|'] = ATTRIBUTE(BINARY, ASSO_LEFT,  PRI_1);
	op_attr['&'] = ATTRIBUTE(BINARY, ASSO_LEFT,  PRI_2);
	op_attr['*'] = ATTRIBUTE(UNARY,  ASSO_RIGHT, PRI_3);
	op_attr['+'] = ATTRIBUTE(UNARY,  ASSO_RIGHT, PRI_3);
	op_attr['?'] = ATTRIBUTE(UNARY,  ASSO_RIGHT, PRI_3);
}


static char *format(const char *re)
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

	stack<char> s = stack<char>(len);

	int pos = 0;
	char *out = (char*)malloc(len);
	
	for (size_t i = 0; i < len; ++i)
	{
		int c = re[i];

		if ( ISOP(c) ) {
			while ( !s.empty() ) {
				unsigned char t = s.top();
				if ( ISASSOLEFT(c) && PRI(c) <= PRI(t) || 
					 ISASSORIGHT(c) && PRI(c) < PRI(t) ) {
					out[pos++] = s.topAndPop();
				}
                else {
                    break;
                }
			}

			s.push(c);
		}
		else if (c == '(') {
			s.push(c);
		}
		else if (c == ')') {
			while (!s.empty() && s.top() != '(') {
				out[pos++] = s.topAndPop();
			}

			// stack is empty but not found '('
			if (s.empty()) {
				printf("mismatched parentheses\n");
				free(out);
				return NULL;
			}

			s.pop(); // pop '(' but not onto the output
		}
		else {
			out[pos++] = c;
		}
	}

	while (!s.empty()) {
		out[pos++] = s.topAndPop();
	}

	return out;
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

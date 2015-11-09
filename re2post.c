#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stack>
#include "operator.h"
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

	formatted[pos] = 0;

	return formatted;
}

char *re2post(const char *re)
{
	char *p = __format(re);
	printf("%s\n", p);

	size_t len = strlen(p);

	std::stack<char> s;

	int pos = 0;
	char *out = (char*)malloc(len+1);
	
	for (size_t i = 0; i < len; i++)
	{
		int c = p[i];

		if ( ISOP(c) ) {
			while ( !s.empty() ) {
				unsigned char t = s.top();
				if ( ISASSOLEFT(c) && PRI(c) <= PRI(t) || 
					 ISASSORIGHT(c) && PRI(c) < PRI(t) ) {
					out[pos++] = s.top();
					s.pop();
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
				out[pos++] = s.top();
				s.pop();
			}

			// stack is empty but not found '('
			if (s.empty()) {
				printf("mismatched parentheses\n");
				goto L_ERR;
			}

			s.pop(); // pop '(' but not onto the output
		}
		else {
			out[pos++] = c;
		}
	}

	while (!s.empty()) {
		out[pos++] = s.top();
		s.pop();
	}
	out[pos] = 0;

	free(p);
	return out;

L_ERR:
	free(out);
	free(p);
	return NULL;
}


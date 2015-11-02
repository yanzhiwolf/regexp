#ifndef __REGEXP_OPERATOR_H__
#define __REGEXP_OPERATOR_H__

extern char op_attr[128];

inline int priority(int c)
{
	return op_attr[c];
}

#endif // __REGEXP_OPERATOR_H__
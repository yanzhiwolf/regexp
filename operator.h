#ifndef __REGEXP_OPERATOR_H__
#define __REGEXP_OPERATOR_H__

extern char op_attr[256];

#define PRIORITY_LVL_1 1
#define PRIORITY_LVL_2 2
#define PRIORITY_LVL_3 3
#define PRIORITY_LVL_4 4
#define PRIORITY_LVL_5 5

#define ARITY_OFFSET 6


inline bool is_operator(int c);

#endif // __REGEXP_OPERATOR_H__
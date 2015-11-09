#ifndef __REGEXP_OPERATOR_H__
#define __REGEXP_OPERATOR_H__

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

extern unsigned char op_attr[128];

void init();

#define ATTRIBUTE(n,a,p) ((n<<6) | (a<<4) | (p<<0))

#define ISOP(c)     	(op_attr[c] > 0)
#define OPNUM(c)		((op_attr[c] & 0xc0) >> 6)
#define ASSO(c)			((op_attr[c] & 0x30) >> 4)
#define ISASSOLEFT(c) 	(ASSO(c) == ASSO_LEFT)
#define ISASSORIGHT(c) 	(ASSO(c) == ASSO_RIGHT) 
#define PRI(c) 			((op_attr[c] & 0x0f) >> 0)


#endif // __REGEXP_OPERATOR_H__
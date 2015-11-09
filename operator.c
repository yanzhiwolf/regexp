#include "operator.h"

unsigned char op_attr[128] = {};

void init()
{
	op_attr['|'] = ATTRIBUTE(BINARY, ASSO_LEFT,  PRI_1);
	op_attr['&'] = ATTRIBUTE(BINARY, ASSO_LEFT,  PRI_2);
	op_attr['*'] = ATTRIBUTE(UNARY,  ASSO_RIGHT, PRI_3);
	op_attr['+'] = ATTRIBUTE(UNARY,  ASSO_RIGHT, PRI_3);
	op_attr['?'] = ATTRIBUTE(UNARY,  ASSO_RIGHT, PRI_3);
}
#include "operator.h"

char op_attr[256] = {};

#define ATTRIBUTE(pri)
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


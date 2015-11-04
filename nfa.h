#ifndef __REGEXP_NFA_H__
#define __REGEXP_NFA_H__

typedef struct State
{
	int c;
	State *out;
	State *out1;
	int lastlist;
} State;


typedef struct Frag
{
	State *state;
	Ptrlist *out;
} Frag;


Ptrlist *list1(State **outp);
Ptrlist *append(Ptrlist *l1, Ptrlist *l2);
void patch(Ptrlist *l, State *s);

State *post2nfa(char *postfix);


#endif // __REGEXP_NFA_H__
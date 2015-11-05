#include <list>
#include <vector>
#include <memory>
#include <stdio.h>
#include <assert.h>
#include "nfa.h"

#define SPLIT 256
#define MATCH 257


FILE *fp = 0;
bool begin(int index)
{
	char buf[256] = {};
	sprintf(buf, "./nfa_%d.dot", index);
	fp = fopen(buf, "a+");
	if (!fp) {
		assert(false);
		return false;
	}

	fprintf(fp, "digraph finite_state_machine {\n");
	//fprintf(fp, "\trankdir=LR;\n");
    fprintf(fp, "\tsize=\"8,5\"\n");
    fprintf(fp, "\tnode [shape = circle];\n");
}

void end()
{
	fprintf(fp, "}\n");

	if (fp != 0) {
		fclose(fp);
	}
}



class State
{
public:
	State() : c(MATCH), out(NULL), out1(NULL), lastlist(0) {}
	State(int c, State *out, State *out1) : c(c), out(out), out1(out1), lastlist(0)
	{
	}

	const char *name() {
		if (c < SPLIT) {
			return (char*)&c;
		} 
		else if (c == SPLIT) {
			return "SPLIT";
		}
		else {
			return "END";
		}
	}
	void draw()
	{
		if (out != 0) {
			fprintf(fp, "%s -> %s\n", name(), out->name());
			out->draw();
		}
		if (out1 != 0) {
			fprintf(fp, "%s -> %s\n", name(), out1->name());
			out1->draw();
		}
	}

	int c;
	State *out;
	State *out1;
	int lastlist;
};
typedef std::list<State**> Ptrlist;

State match;

class Frag
{
public:
	Frag(State *start, Ptrlist out) : start(start), out(out)
	{
	}

	void patchTo(State *s)
	{
		for (auto it = out.begin(); it != out.end(); it++) {
			**it = s;
		}
	}

	const char *name() {
		if (start) {
			return start->name();
		}
		return "";
	}
	void draw() {
		if (start) {
			start->draw();
		}
	}
	State *start;
	Ptrlist out;
};
typedef std::shared_ptr<Frag> FragPtr;
typedef std::vector<FragPtr> FragStack;

static FragPtr newFrag(State *s, Ptrlist out)
{
	return FragPtr(new Frag(s, out));
}

Ptrlist list1(State **outp)
{
	return Ptrlist(1, outp);
}

Ptrlist append(const Ptrlist &l1, const Ptrlist &l2)
{
	Ptrlist lst;
	std::copy(l1.begin(), l1.end(), lst.end());
	std::copy(l2.begin(), l2.end(), lst.end());
	return lst;
}

void drawNfa(const FragStack &stk)
{
	fprintf(fp, "\trankdir=LR;\n");
	fprintf(fp, "\tnode[shape=record]\n");
	fprintf(fp, "struct1: [label=\"");
	for (size_t i = 0; i < stk.size(); i++) {
		fprintf(fp, "<f%d> %d|", i, i);
	}
	fprintf(fp, "\"];");
	fprintf(fp, "\trankdir=TB;\n");
	fprintf(fp, "\tnode [shape=circle];\n");

	for (size_t i = 0; i < stk.size(); i++) {
		stk[i]->draw();
		fprintf(fp, "struct1:f%d -> %s\n", i, stk[i]->name());
	}
}

State *post2nfa(char *postfix)
{
	char *p;
	FragStack stk;

	int index = 0;
	for (char *p = postfix; *p; p++, index++) {
		switch (*p) {
		case '&':
			{
				FragPtr f2 = stk.back();
				stk.pop_back();
				FragPtr f1 = stk.back();
				stk.pop_back();
				f1->patchTo(f2->start);
				stk.push_back(newFrag(f1->start, f2->out));
			} break;
		case '|':
			{
				FragPtr f2 = stk.back();
				stk.pop_back();
				FragPtr f1 = stk.back();
				stk.pop_back();
				State *s = new State(SPLIT, f1->start, f2->start);
				stk.push_back(newFrag(s, append(f1->out, f2->out)));
			} break;
		case '?':
			{
				FragPtr f = stk.back();
				stk.pop_back();
				State *s = new State(SPLIT, f->start, NULL);
				stk.push_back(newFrag(s, append(f->out, list1(&s->out1))));
			} break;
		case '*':
			{
				FragPtr f = stk.back();
				stk.pop_back();
				State *s = new State(SPLIT, f->start, NULL);
				f->patchTo(s);
				stk.push_back(newFrag(s, list1(&s->out1)));
			} break;
		case '+':
			{
				FragPtr f = stk.back();
				stk.pop_back();
				State *s = new State(SPLIT, f->start, NULL);
				f->patchTo(s);
				stk.push_back(newFrag(f->start, list1(&s->out1)));
			} break;
		default:
			{
				State *s = new State(*p, NULL, NULL);
				stk.push_back(newFrag(s, list1(&s->out)));
			} break;
		}

		begin(index);
		drawNfa(stk);
		end();
	}

	assert(stk.size() == 1);
	FragPtr f = stk.back();
	stk.pop_back();
	f->patchTo(&match);

	return f->start;
}
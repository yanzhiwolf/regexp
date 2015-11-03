#ifndef STACK_H_
#define STACK_H_

#include <assert.h>
#include <memory.h>

template<typename T>
class stack
{
public:
	stack(int cap) : capacity(cap), count(0)
	{
		elements = (char*)malloc(sizeof(T)*capacity);
	}

	~stack()
	{
		free(elements);
		capacity = 0;
		count = 0;
	}

	bool empty() const
	{
		return count == 0;
	}

	T top() const 
	{
		assert(!empty());
		return this->elements[this->count-1];
	}

	T topAndPop()
	{
		assert(!empty());
		T t = top();
		pop();
		return t;
	}

	void push(T x)
	{
	    if (this->count >= this->capacity) {
	        elements = (char*)realloc(elements, capacity*2);
	        capacity *= 2;
	    }

	    this->elements[this->count++] = x;
	}

	void pop()
	{
		assert(!empty());
	    this->count--;
	}

private:
	int capacity;
	int count;
	T *elements;
};

#endif


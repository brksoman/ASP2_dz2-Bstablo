#ifndef __STACK_
#define __STACK_

const int MAX_STACK_HEIGHT = 100;

class Stack {
	int a[MAX_STACK_HEIGHT];
	int h = 0;

public:
	bool is_empty() const { return h == 0; }
	bool is_full() const { return h == MAX_STACK_HEIGHT; }
	bool push(const int x) {
		if (is_full()) return false;
		a[h++] = x;
		return true;
	}
	int pop() {
		if (!is_empty()) return a[--h];
		return 0;
	}
	int top() {
		if (!is_empty()) return a[h];
		return 0;
	}
};

#endif
#include "Stack.h"
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>


using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int main () {

	void* mem = mmap(0, sizeof(Stack<char>), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	Stack<char> * s = new (mem) Stack<char>();


	pid_t pp = fork();
	if (pp==0) {
		producteur(s);
		return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
	}

	wait(0);
	wait(0);

	//delete s;
	s->~Stack();
	munmap(mem, sizeof(Stack<char>));
	return 0;
}


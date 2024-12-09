#pragma once

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring> // size_t,memset
#include <fcntl.h>
#include <semaphore.h>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T* tab [STACKSIZE];
	size_t sz;
	bool finished;
	//declaration de semaphores; qui vont etre utilises comme des semaphores anonymes.
	sem_t sem_read;
	sem_t sem_write;
public :
	Stack () : sz(0), finished(false) { 
		memset(tab,0,sizeof tab) ;

		//initialisation des semaphores
		if((sem_init(&sem_read,1,0))){
			perror("sem_read init");
			exit(1);
		}
		if(sem_init(&sem_write,1,STACKSIZE)){
			perror("sem_write init");
			exit(1);
		}
	}

	~Stack() {
		
		sem_destroy(&sem_read);
		sem_destroy(&sem_write);
	}

	//Exercice 2 ; Q1
	T pop () {
		// P(sem_read)
		sem_wait(&sem_read);
		
		T toret = *tab[--sz];

		//V(sem_write)
		sem_post(&sem_write);
		return toret;
	}

	void push(T elt) {
		//P(sem_write)
		sem_wait(&sem_write);
		
		tab[sz++] = new T(elt);
		//V(sem_read)
		sem_post(&sem_read);
	}

	inline bool isFinished() const { return finished; }
	inline void setFinished(bool f) { finished = f; }
};

}

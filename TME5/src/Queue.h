#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <cstddef>
#include <mutex>
#include <condition_variable>
#include <cstring>

#include <deque>

template <typename T>
class Queue {
	T ** tab;
	const size_t allocsize;
	size_t begin;
	size_t sz;

	std::deque<T*> queue;

	mutable std::mutex m;
	std::condition_variable cond_push;
	std::condition_variable cond_pop;

	bool blocking ; 
 	
	public :
	Queue (size_t maxsize, bool b=true) :allocsize(maxsize),begin(0),sz(0),blocking(b) {
		tab = new T* [maxsize];
 		memset(tab, 0, maxsize * sizeof(T*));
 	}
	
	size_t size() const {
		std::unique_lock<std::mutex>l(m);	
	    return sz;
	}

 	T* pop () {	
		std::unique_lock<std::mutex>l(m);
		
		while(empty() && blocking){
			cond_pop.wait(l);
		}	
		if(!blocking && empty()){
			return nullptr;
		}

 		/*T* ret = tab[begin];
		tab[begin] = nullptr;
 		sz--;
 		begin = (begin+1) % allocsize;*/
	 	cond_push.notify_all();
		
		T* ret = queue.front();
		queue.pop_front();
		return ret; 
 	}

 	void push (T* elt) {
		std::unique_lock<std::mutex>l(m);	
 		while(full()){
			cond_push.wait(l);
		}

		//tab[(begin + sz)%allocsize] = elt;
 		//sz++;

		queue.push_back(elt);
		cond_pop.notify_all();
		//return true;
 	}

 	~Queue() {
 		for (size_t i = 0; i < sz ; i++) {
 			size_t ind = (begin + i) % allocsize;
 			delete tab[ind];
 		}
 		delete[] tab;
 	}
	
	inline bool full() const{

		//return sz==allocsize; 
		return false;
	}

	inline bool empty() const{	
		return !queue.size();
	}

	void setBlocking(bool b){
		blocking = b;
		if(!blocking)
			cond_pop.notify_all(); //awake all threads sleeping on pop if set to non blocking
	}
};
#endif /* SRC_QUEUE_H_ */


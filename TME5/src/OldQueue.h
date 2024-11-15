#ifndef SRC_OLDQUEUE_H_
#define SRC_OLDQUEUE_H_

#include <cstdlib>
#include <mutex>

namespace pr {

// MT safe version of the Queue, non blocking.
template <typename T>
class OldQueue {
	T ** tab;
	const size_t allocsize;
	size_t begin;
	size_t sz;
	mutable std::mutex m;

	// fonctions private, sans protection mutex
	bool empty() const {
		return sz == 0;
	}
	bool full() const {
		return sz == allocsize;
	}
public:
	OldQueue(size_t size) :allocsize(size), begin(0), sz(0) {
		tab = new T*[size];
		memset(tab, 0, size * sizeof(T*));
	}
	size_t size() const {
		std::unique_lock<std::mutex> lg(m);
		return sz;
	}
	T* pop() {
		std::unique_lock<std::mutex> lg(m);
		if (empty()) {
			return nullptr;
		}
		auto ret = tab[begin];
		tab[begin] = nullptr;
		sz--;
		begin = (begin + 1) % allocsize;
		return ret;
	}
	bool push(T* elt) {
		std::unique_lock<std::mutex> lg(m);
		if (full()) {
			return false;
		}
		tab[(begin + sz) % allocsize] = elt;
		sz++;
		return true;
	}
	~OldQueue() {
		// ?? lock a priori inutile, ne pas detruire si on travaille encore avec
		for (size_t i = 0; i < sz; i++) {
			auto ind = (begin + i) % allocsize;
			delete tab[ind];
		}
		delete[] tab;
	}
};

}

#endif /* SRC_OLDsQUEUE_H_ */

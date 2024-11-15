#pragma once

#include "Queue.h"
#include "Job.h"
#include "Barrier.h"
#include <vector>
#include <thread>
#include <atomic>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;

	std::atomic_bool stopped;

	void thread_worker();
public:
	
	Pool(int qsize)
	:queue(qsize), threads(std::vector<std::thread>()), stopped(false)
	{}

	void start (size_t nbthread);
	void submit (Job * job) ;
	void stop() ;
	~Pool() ;
};

}

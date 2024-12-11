#include "Pool.h"
#include "Job.hpp"
#include <thread>

namespace pr {

    using namespace std; 

    void Pool::thread_worker(){
        while(true){
            Job* j = queue.pop();
            if(j){
                j->run();
                //barrier.increment_finished();
            }else{
                std::cout << "th exit" << std::endl ;
                return ; 
            }
        }
    }

    void Pool::start(size_t nbthreads){
        for(unsigned i = 0 ; i < nbthreads ; i++){
            threads.push_back(thread(&Pool::thread_worker, this));
        }
    }
    
    void Pool::submit(Job* job){
        queue.push(job);
        //barrier.increment_submitted();
    }

    void Pool::stop(){
        stopped = true ; 
        queue.setBlocking(false);

        for(auto & t : threads){
            t.join();
        }
    }
    Pool::~Pool(){
        //barrier.wait();
       //stop();
    }
}
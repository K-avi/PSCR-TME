#pragma once 

#include <cstdint>
#include <atomic>
#include <iostream>
namespace pr {
    class Barrier{
        std::atomic<std::size_t> submitted_tasks; 
        std::atomic<std::size_t> finished_tasks; 

        public: 
            Barrier(std::size_t submitted): submitted_tasks(submitted), finished_tasks(0){}

            //inline void increment_submitted(){submitted_tasks++;}
            inline void increment_finished(){finished_tasks++;}
            void wait(){
                while(! (submitted_tasks.load() >= finished_tasks.load())){
                    for(uint_fast32_t i = 0; i < 10000 ; i ++){
                        if(i%10000 == 9999)
                            std::cout << "nb taches "<< finished_tasks << "submitted " << submitted_tasks
                                      << std::endl ; 
                    }
                }
            }
            inline bool equals(){ return submitted_tasks.load() == finished_tasks.load() ;} 

    };
}
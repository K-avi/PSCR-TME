

#include <bits/types/sigset_t.h>
#include <csignal>
#include <cstddef>
#include <ctime>
#include <sys/wait.h>

//question 7 : 
int wait_til_pid(pid_t pid){
    pid_t ret; 
    while( ( ret = wait(nullptr)) != pid && ret != -1);
    return ret ;
}

// question 8 sans signaux 
int wait_til_pid_signalless(pid_t pid, int sec){

    time_t start ;
    time(&start);
    
    time_t end = start + sec ;
    
    pid_t ret; 

    while( (ret = wait(nullptr)) != pid && ret != -1 && start < end){
        time(&start);
    }
    if(start >= end ) return 0 ; 
    return ret ; 
}
//question 8 correcte :  

pid_t global_ret = 0 ; 
pid_t global_arg = 0 ; 

void disarm_alarm(int signal){

    if(global_arg == waitpid(-1, nullptr, 0) ){
        alarm(0);
        global_ret = global_arg;
    }
}

int wait_til_pid(pid_t pid, int sec){
    
    global_arg = pid ; 

    struct sigaction action = {} ; 

    action.sa_handler = disarm_alarm;
    action.sa_flags = 0 ; 
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGCHLD);
    sigaction(SIGCHLD, &action ,nullptr);

    alarm(sec);

    sigset_t new_mask; 
    sigaddset(&new_mask, SIGCHLD);
    sigaddset(&new_mask,  SIGCHLD);
    sigsuspend(&new_mask);

    return global_ret ; 
}


//TME 7 : question 1

#include <cstddef>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std; 

int main(int argc, char** argv){

    //traitement d'erreur 
    char* args_cmd1[argc]; //is always smaller than argc by definition and so on 
    char* args_cmd2[argc] ;
   
   
    char** arg = argv + 1 ; 
    unsigned cpt = 0 ; 

    while(**arg != '|' ){
        //cout << "arg is " << *arg << endl ; 
        args_cmd1[cpt++] = *arg ;  
        arg++; 
    }
    args_cmd1[cpt] = nullptr ; 
    arg++ ; 
    cpt = 0 ; 

    while(arg != argv + argc){
       // cout << "loop 2 arg is " << *arg << endl; 
        args_cmd2[cpt++] = *arg; 
        arg++; 
    }
    args_cmd2[cpt] = nullptr ;

    pid_t f1, f2 ; 
    int pipedes[2] ;
    if(pipe(pipedes)) return 3 ;

    f1 = fork();
    if(f1 == 0){
        close(pipedes[0]);
        dup2(pipedes[1],1);
        close(pipedes[1]);
        execvp(args_cmd1[0],args_cmd1);
        return 1 ; 
    }

    f2 = fork();
    if(f2 == 0){
        close(pipedes[1]);
        dup2(pipedes[0],0);
        close(pipedes[0]);
        execvp(args_cmd2[0],args_cmd2);
        return 2 ; 
    }

    close(pipedes[0]);
    close(pipedes[1]);
    waitpid(f1,nullptr,0);
    waitpid(f2,nullptr,0); 
   
    return 0 ;
}

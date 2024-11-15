#include <csignal>
#include <ctime>
#include <cstdlib>
#include "rsleep.h"
#include <iostream>
#include <unistd.h>

#define DEFAULT_HP 3 

unsigned hp = DEFAULT_HP ;

using namespace std; 

void sigint_gestionnaire(int signal){
    //affiche pv restants
    cout << "processus : " << getpid() << " hp restants : " << --hp << endl; 
    //si mort; renvoie 1 
    if(!hp){
      cout << "fin du processus " << getpid() << endl ; 
      exit(1);
    }
}

void attaque(pid_t adversaire){
    //installe gestionnaire de sigint
    struct sigaction action = {} ; 

    action.sa_handler = sigint_gestionnaire;
    action.sa_flags = 0 ; 
    sigemptyset(&action.sa_mask);

    sigaction(SIGINT, &action ,nullptr);

    //effectue l'attaque
    if(kill(adversaire, SIGINT) == -1){
      exit(0);
    }

    //s'endort
    randsleep();//phase 3 
}

void defense(){
    struct sigaction action = {} ; 

    action.sa_handler = SIG_IGN;
    action.sa_flags = 0 ; 
    sigemptyset(&action.sa_mask);
    
    sigaction(SIGINT, &action ,nullptr);

    randsleep();
}

//q6 : 
void defense_handler_luke(int signal){
    cout << "coup pare" << endl; 
}
void defense_luke(){

    //positionne handler sur "coup paré"
    struct sigaction action = {} ; 

    action.sa_handler = defense_handler_luke;
    action.sa_flags = 0 ; 
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action ,nullptr);

    //masquer les signaux 
    sigset_t new_mask ; 
    //sigemptyset(&new_mask) ;
    sigfillset(&new_mask);
    sigprocmask(SIG_BLOCK, &new_mask,NULL);

    //s'endormir
    randsleep();
    //attendre la réception 
    sigemptyset(&new_mask);
    sigsuspend(&(new_mask));
}

void combat_luke(pid_t adversaire){
    while(true){
      defense_luke();
      attaque(adversaire);
    }
}


void combat(pid_t adversaire){
    while(true){
      defense();
      attaque(adversaire);
    }
}



void randsleep() {
  int r = rand();
  double ratio = (double)r / (double) RAND_MAX;
  struct timespec tosleep;
  tosleep.tv_sec =0;
  // 300 millions de ns = 0.3 secondes
  tosleep.tv_nsec = 300000000 + ratio*700000000;
  struct timespec remain;
  while ( nanosleep(&tosleep, &remain) != 0) {
    tosleep = remain;
  }
}

int main(){

  pid_t fils ;
  if( (fils = fork()) ){
    srand(time(NULL)+ getpid());
    cout << "debut combat pere : " << getpid() << endl ; 
    combat(fils);
  }else{
    srand(time(NULL) + getpid());
    cout << "debut combat fils : " << getpid() << endl ; 
    combat_luke(getppid());
  }

  return 0; 
}

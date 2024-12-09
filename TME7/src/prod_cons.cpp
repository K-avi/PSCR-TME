//TME 7 : Exercice 2 :
#include "Stack.hpp"
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>


using namespace std;
using namespace pr;

#define DEFAULT_PROD 4
#define DEFAULT_CONS 4

static bool finished = false;

static void sig_handler(int sig){
	finished = true;
}

//pas de gestion de concurrence
void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c) && !stack->isFinished()) { 
		stack->push(c);
	}
}

//pas de gestion de concurrence
void consomateur (Stack<char> * stack) {
	while (! stack->isFinished()) {
		char c = stack->pop();
		cout << c << endl ;
	}
}

int main (int argc, char** argv) {

	//on récupère le nombre de producteurs(argv1) et de consommateurs(argv2)
	int nb_prod , nb_cons;
	if(argc < 3){//if less than 3 arguments set to default values
		cerr << "Usage : " << argv[0] << " nb_prod nb_cons" << endl;
		nb_prod = DEFAULT_PROD;
		nb_cons = DEFAULT_CONS;
	}else{ //parse values and check if they are valid
		if((nb_prod = atoi(argv[1])) <= 0)
			nb_prod = DEFAULT_PROD;
		if((nb_cons = atoi(argv[2])) <= 0)
			nb_cons = DEFAULT_CONS;		
	}

	//create shared memory
	//code from the presentation
	int fd;
	/* Creer le segment de mémoire partagée nommé, ouverture en R/W */
	if ((fd = shm_open("/tmp_monshm", O_RDWR | O_CREAT | O_EXCL,0600)) == -1) {
		perror("shm_open");
		exit(1);
	}
	/* Allouer au segment une taille pour stocker un entier */
	if (ftruncate(fd, sizeof(Stack<char>) )== -1) {
		perror("ftruncate");
		exit(1);
	}

	//map the shared memory
	void* mem ; 
	if(!(mem = mmap(0, sizeof(Stack<char>), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))){
		perror("mmap");
		exit(1);
	}

	Stack<char> * s = new (mem) Stack<char>();

	//create producers and consumers
	for(int i = 0 ; i < nb_prod ; i++){
		pid_t p = fork();
		if(p == 0){
			producteur(s);
			return 0;
		}
	}

	for(int i = 0 ; i < nb_cons ; i++){
		pid_t c = fork();
		if(c == 0){
			consomateur(s);
			return 0;
		}
	}

	//set the signal handler
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, nullptr);

	while(!finished){
		sleep(1);
	}
	//set the finished flag to true
	//since the stack is shared between all processes
	//we need to set the flag to true to stop the other processes
	//(or at least I think so)
	s->setFinished(true);

	//wait for all children to finish
	for(int i = 0 ; i < nb_prod + nb_cons ; i++){
		wait(nullptr);
	}

	//delete s;
	s->~Stack();

	munmap(mem, sizeof(Stack<char>));
	//close the shared memory
	shm_unlink("/tmp_monshm");
	return 0;
}


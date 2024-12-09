#include <iostream>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>


using namespace std; 
int main () {
	const int N = 3;

	unsigned nb_fils = 0 ; 
	std::cout << "main pid=" << getpid() << std::endl;
	pid_t pid ;

	for (int i=1, j=N; i<=N && j==N ; i++ ) {
		
		if( (pid = fork()) == 0 ){

			cout << "apres 1er fork je suis pid=" << getpid() << "mon pere est ppid=" << getppid() << endl ;  
		
			nb_fils = 0 ; 
			std::cout << " i:j " << i << ":" << j << std::endl;
			for (int k=1; k<=i && j==N ; k++) {
				if ( (pid = fork()) == 0) {

					cout << " apres 2e fork je suis pid=" << getpid() << "mon pere est ppid=" << getppid() << endl ;  

					nb_fils = 0 ;

					j=0;
					std::cout << " k:j " << k << ":" << j << std::endl;
				}else{
					nb_fils++; 
					//break;
				}
			}
		}else{
			nb_fils++; 
			break;
		}
	}

	for(unsigned i = 0 ; i < nb_fils ; i++){
		wait(nullptr);
	}

	//cout << "nbfils : " << nb_fils << endl;  
	
	return 0;
}

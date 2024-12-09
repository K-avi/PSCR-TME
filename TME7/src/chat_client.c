#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include "chat_common.h"

#define DEFAULT_SERVER_NAME "semserver"
#define DEFAULT_CLIENT_NAME "semclient"

bool finished = 0 ; 

void signal_handler(int sig){
    if(sig == SIGINT){
        finished = true;
    }
}


int server_send(char* server_name, struct message *m){


    int server_shm_fd = shm_open(server_name, O_RDWR, 0666);
    if(server_shm_fd == -1){
        perror("shm_open server send");
        printf("server name: %s\n", server_name);
        return -1;
    }

    struct myshm *server_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, server_shm_fd, 0);
    if(server_shm == MAP_FAILED){
        perror("mmap");
        return -1;
    }

    sem_wait(&server_shm->sem_write);

    server_shm->messages[server_shm->idx] = *m;

    //server_shm->write = (server_shm->write + 1) % MAX_MESS;

    sem_post(&server_shm->sem_read);
    
    return 0;
}


void init_myshm(struct myshm* shm){
    shm->idx = 0 ; 
    shm->nb = 0 ; 
    shm->read = 0 ; 

   
    sem_init(&shm->sem_read, 1, 0);
    sem_init(&shm->sem_write, 1, 1) ;
}

void* client_read(void* arg){

    char* client_name = (char*)arg;
    int client_shm_fd = shm_open(client_name, O_RDWR | O_CREAT , 0666);
    if(client_shm_fd == -1){
        perror("shm_open client_read");
        return (void*)-1;
    }

    ftruncate(client_shm_fd, sizeof(struct myshm));

    struct myshm *client_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, client_shm_fd, 0);
    if(client_shm == MAP_FAILED){
        perror("mmap");
        return (void*)-1;
    }

    init_myshm(client_shm);
   

    while(!finished){

        sem_wait(&client_shm->sem_read);

        char *buff = client_shm->messages[client_shm->idx].content;
        printf("Message received: %s\n", buff);

        client_shm->idx = (client_shm->idx+1)% MAX_MESS;

        sem_post(&client_shm->sem_write);
    }

    //unlik memory
    shm_unlink(client_name);
    munmap(client_shm, sizeof(struct myshm));
    return (void*)0;
}



struct param_thread{
    char* server_name;
    char* client_name;
};
int client_write(void* arg){

    struct param_thread* p = (struct param_thread*)arg;

    char* server_name = p->server_name;
    char* client_name = p->client_name;

    
    struct message m;
    m.type = T_CONNEXION;
    memset(m.content, 0, TAILLE_MESS);
    memcpy(m.content, client_name, strlen(client_name));
    server_send(server_name, &m);
    
    while(!finished){
        struct message m;
        m.type = T_BROADCAST;
        fgets(m.content, TAILLE_MESS, stdin);
        server_send(server_name, &m);
    }

    memcpy(m.content, client_name, strlen(client_name));
    server_send(server_name, &m);

    return 0;
}


int main(int argc, char** argv){

    char *server_name, *client_name;

    if(argc <= 3){
        client_name = malloc(256*sizeof(char));
        snprintf(client_name, 255, "%s%u", DEFAULT_CLIENT_NAME, (unsigned)getpid());
        server_name = DEFAULT_SERVER_NAME;
    }else{
        client_name = strdup(argv[1]); //to avoid invalid free
        server_name = argv[2];
    }

    //setup signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigset_t mask;
    sigemptyset(&mask);
    sa.sa_mask = mask;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);


    pthread_t read_thread;
    pthread_t write_thread;

    //create the client read thread
    pthread_create(&read_thread, NULL, (void*)client_read, client_name);

    //create the client write thread
    struct param_thread p;
    p.server_name = server_name;
    p.client_name = client_name;
    pthread_create(&write_thread, NULL, (void*)client_write, &p);

    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);
    
    free(client_name);
    return 0;
}

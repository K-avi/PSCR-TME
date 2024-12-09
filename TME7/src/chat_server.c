#include <bits/types/sigset_t.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "chat_common.h"

#define DEFAULT_NAME "semserver"


bool finished;

typedef struct server{

    char* name;
    struct myshm *server_shm;
    
    unsigned nb_users;
    char* client_tab_shm_name[MAX_USERS];
    struct myshm *client_tab_shm[MAX_USERS];

}server;

void server_init(server*s, char* name){
    s->name = strdup(name);
    s->nb_users = 0;
    s->server_shm = NULL;
    for(int i = 0 ; i < MAX_USERS ; i++){
        s->client_tab_shm_name[i] = NULL;
        s->client_tab_shm[i] = NULL;
    }


    int server_shm_fd = shm_open(name, O_RDWR | O_CREAT, 0666);
    if(server_shm_fd == -1){
        perror("shm_open server_init");
        return;
    }

    ftruncate(server_shm_fd, sizeof(struct myshm));

    s->server_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, server_shm_fd, 0);
    if(s->server_shm == MAP_FAILED){
        perror("mmap");
        return;
    }

    s->server_shm->idx = 0;
    s->server_shm->nb = 0;
    s->server_shm->read = 0;
    s->server_shm->write = 0;
    sem_init(&s->server_shm->sem_read, 1, 0);
    sem_init(&s->server_shm->sem_write, 1, 1);

    return ;
}

void server_destroy(server*s){
    sem_destroy(&s->server_shm->sem_read);
    sem_destroy(&s->server_shm->sem_write);
    munmap(s->server_shm, sizeof(struct myshm));
    shm_unlink(s->name);
    free(s->name);
    return;
}

static int connect_user(server*s, struct message *m){

    if(m->type != T_CONNEXION){
        printf("Error: message type is not T_CONNEXION\n");
        return -1;
    }
    if(s->nb_users >= MAX_USERS){
        printf("Error: too many users\n");
        return -1;
    }

    char *client_name = m->content;
    
    //error if client is already connected
    for(int i = 0 ; i < MAX_USERS ; i++){
        if(!s->client_tab_shm[i]){
            continue;
        }
        if(!strcmp(client_name,s->client_tab_shm_name[i])){
            return -1; 
        }
    }
    
    int client_shm_fd = shm_open(client_name, O_RDWR, 0666);
    if(client_shm_fd == -1){
        perror("shm_open connect_user");
        fprintf(stderr, "client name : %s\n", client_name);

        return -1;
    }

    struct myshm *client_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, client_shm_fd, 0);
    if(client_shm == MAP_FAILED){
        perror("mmap");
        return -1;
    }

    for(int i = 0; i < MAX_USERS; i++){
        if(s->client_tab_shm_name[i] == NULL){
            s->client_tab_shm_name[i] = strdup(client_name);
            s->client_tab_shm[i] = client_shm ;
            s->nb_users++;
            
            return 0;

        }
    }
    return -1;
}

static int broadcast_message(server*s, struct message *m){

    //send the message to all the users

    for(int i = 0 ; i < MAX_USERS ; i++){
        if(s->client_tab_shm_name[i] != NULL){
            sem_wait(&s->client_tab_shm[i]->sem_write);
            s->client_tab_shm[i]->messages[s->client_tab_shm[i]->idx] = *m;
            sem_post(&s->client_tab_shm[i]->sem_read);
        }
    }
    return 0;
}

static int disconnect_user(server*s, struct message *m){
    if(m->type != T_DISCONNECT){
        printf("Error: message type is not T_CONNEXION\n");
        return -1;
    }

    char *client_name = m->content;
    
    //error if client is already connected
    for(int i = 0 ; i < MAX_USERS ; i++){
        if(strcmp(client_name,s->client_tab_shm_name[i])){
            s->client_tab_shm_name[i] = NULL;
            s->client_tab_shm[i] = NULL;
            s->nb_users--;

            return 0;
        }
    }

    return -1;
}

static int read_message(server*s, struct message *m){
    
    switch (m->type) {
        case T_CONNEXION:
            connect_user(s, m);
            break;
        case T_BROADCAST:
            broadcast_message(s, m);
            break;
        case T_DISCONNECT:
            disconnect_user(s, m);
            break;
        default:
            printf("Unknown message type\n");
            break;
    }

    return 0;
}

int server_receive(server* s){

    struct myshm *server_shm = s->server_shm;
    sem_wait(&server_shm->sem_read);
    if(finished){
        return -1;
    }//check if the server is finished before reading the message

    struct message m = server_shm->messages[server_shm->idx];
    printf("message received by server is : %s\n", m.content);
    read_message(s, &m);


    server_shm->read++;
    server_shm->write--;
 
    server_shm->idx = (server_shm->idx + 1) % MAX_MESS;

    sem_post(&server_shm->sem_write);

    return 0;
}

int server_send(char* server_name, struct message *m){


    int server_shm_fd = shm_open(server_name, O_RDWR, 0666);
    if(server_shm_fd == -1){
        perror("shm_open");
        return -1;
    }

    struct myshm *server_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, server_shm_fd, 0);
    if(server_shm == MAP_FAILED){
        perror("mmap");
        return -1;
    }

    sem_wait(&server_shm->sem_write);

    server_shm->messages[server_shm->write] = *m;

    server_shm->write = (server_shm->write + 1) % MAX_MESS;

    sem_post(&server_shm->sem_read);
    
    return 0;
}

void signal_handler(int sig){
    if(sig == SIGINT){
        finished = true;
    }
}

int main(int argc, char** argv){

    finished = false;
    //setup signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigset_t mask;
    sigemptyset(&mask);
    sa.sa_mask = mask;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    

    char *name;
    //get the name of the shared memory
    if(argc < 2){
        name = DEFAULT_NAME;
    }else{
        name = argv[1];
    }

    server s;
    server_init(&s, name);

    while(!finished){
       server_receive(&s);
    }

    server_destroy(&s);

    return 0; 
}

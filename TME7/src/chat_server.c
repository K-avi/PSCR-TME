#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "chat_common.h"

#define DEFAULT_NAME "semserver"

typedef struct server{

    char* name;
    struct myshm *server_shm;
    
    unsigned nb_users;
    char* client_tab_shm_name[MAX_USERS];
    struct myshm *client_tab_shm[MAX_USERS];

}server;
server s;


static void server_init(server* s, char* name){
    if(! (s&&name)) return;

    s->name = strdup(name);
    s->nb_users = 0;
    
    //create the shared memory
    int fd;
    if((fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600)) == -1){
        perror("shm_open");
        exit(1);
    }
    //resize the shared memory
    if(ftruncate(fd, sizeof(struct myshm)) == -1){
        perror("ftruncate");
        exit(1);
    }

    //map the shared memory
    s->server_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //name for convenience
    struct myshm * server_shm = s->server_shm;
    //initialize the shared memory
    server_shm->read = 0;
    server_shm->write = 0;
    server_shm->nb = 0;

    //initialize the named semaphore
    sem_open("/sem", O_CREAT, 0600, 1);

    return;
}

static void server_free(server* s){
    if(!s) return;

    //free the shared memory
    munmap(s->server_shm, sizeof(struct myshm));
    
    //free the client shared memory
    for(int i = 0; i < MAX_USERS; i++){
        if(s->client_tab_shm[i] != NULL){
            munmap(s->client_tab_shm[i], sizeof(struct myshm));
            shm_unlink(s->client_tab_shm_name[i]);
            //do I need to unlink the shared memory? Isn't it done by the client?
            free(s->client_tab_shm_name[i]);
        }
    }

    s->nb_users = 0;

    //destroy the semaphore
    sem_unlink("/sem");

    //free the server shared memory
    munmap(s->server_shm, sizeof(struct myshm));
    shm_unlink(s->name);

    free(s->name);

    return;
}

static int connect_user(struct server *s,  struct message *m){

    if(s->nb_users == MAX_USERS) return -1 ;
    if(m->type != T_CONNEXION) return -1;

    //search for a free slot
    for(int i = 0; i < MAX_USERS; i++){
        if(s->client_tab_shm_name[i] == NULL){

            //copy the name of the client in the server
            s->client_tab_shm_name[i] = strdup(m->content);

            //create the shared memory for the client
            int fd = shm_open(m->content, O_RDWR | O_CREAT | O_EXCL, 0600);
            if(fd == -1){
                perror("shm_open");
                return -1;
            }

            if(ftruncate(fd, sizeof(struct myshm)) == -1){
                perror("ftruncate");
                return -1;
            }

            //map the shared memory
            s->client_tab_shm[i] = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            
            //increment the number of users
            s->nb_users++;
            return 0;
        }
    }
    return -1;
}


static int broadcast_message(server *s, struct message *m){

    if(m->type != T_BROADCAST) return -1; 

    //copy message in clients
    for(int i = 0 ; i < MAX_USERS ; i++){
        if(s->client_tab_shm[i] != NULL){

            //wait for the semaphore
            sem_wait(&(s->client_tab_shm[i]->sem));
            s->client_tab_shm[i]->messages[s->client_tab_shm[i]->write] = *m;
            s->client_tab_shm[i]->write++;
            s->client_tab_shm[i]->nb++;
            //signal the semaphore
            sem_post(&(s->client_tab_shm[i]->sem));
        }
    }
    return 0;
}

static int disconnect_user(server* s, struct message* m){
    
    if(m->type != T_DISCONNECT) return -1;
    
    char* name = m->content;

    //search for the user
    for(int i = 0 ; i < MAX_USERS; i++){
        if(s->client_tab_shm_name[i] != NULL && strcmp(s->client_tab_shm_name[i], name) == 0){
            //free the shared memory
            munmap(s->client_tab_shm[i], sizeof(struct myshm));
            shm_unlink(s->client_tab_shm_name[i]);
            free(s->client_tab_shm_name[i]);
           
            //reset the client fields
            s->client_tab_shm_name[i] = NULL;
            s->client_tab_shm[i] = NULL;
            s->nb_users--;

            return 0;
        }
    }
    return -1;
}

int server_send_message(struct message *client_message){
    
    switch(client_message->type){
        case T_CONNEXION:
            return connect_user(&s, client_message);
        case T_BROADCAST:
            return broadcast_message(&s, client_message);
        case T_DISCONNECT:
            return disconnect_user(&s, client_message);
        default:
            return -1;
    }

}


int server_receive_message(){

    
    return 0;
}

int main(int argc, char** argv){

    char *name;
    //get the name of the shared memory
    if(argc < 2){
        name = DEFAULT_NAME;
    }else{
        name = argv[1];
    }

    server_init(&s, name);

    

    return 0; 
}

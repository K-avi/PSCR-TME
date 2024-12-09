#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "chat_common.h"

#define DEFAULT_SERVER_NAME "semserver"
#define DEFAULT_CLIENT_NAME "semclient"

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

    int server_shm_fd = shm_open(server_name, O_RDWR, 0666);
    if(server_shm_fd == -1){
        perror("shm_open");
        free(client_name);
        return -1;
    }

    struct myshm *server_shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, server_shm_fd, 0);
    if(server_shm == MAP_FAILED){
        perror("mmap");
        free(client_name);
        return -1;
    }

    //send connextion request
    struct message m;
    m.type = T_CONNEXION;
    //copy the client name in the message
    strncpy(m.content, client_name, TAILLE_MESS);

    if(connect_user(server_shm, &m) == -1){
        perror("Error while connecting to the server\n");
        free(client_name);
        return -1;
    }

    free(client_name);
    return 0;
}

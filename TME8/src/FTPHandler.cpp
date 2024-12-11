#include "FTPHandler.hpp"
#include <cstdio>
#include <sstream>
#include <unistd.h>
#include <dirent.h>

using namespace std;

void pr::FTPHandlerServer::handleConnection(Socket s) {
    //read the command from the client on the socket
    
    char buff[256];
    int socketfd = s.getFD();
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_name.c_str())) == NULL) {
        perror ("can not open the directory");
        return;
    }

    while (true) {
        read(socketfd, buff,255);
        cout << "Received command : " << buff << endl;

        if(!strncmp(buff, "LIST", 4)){
            //list the files in the directory
            cout << "LIST" << endl;
            stringstream files ;
            //build the list of files
            while ((ent = readdir (dir)) != NULL) {
                files << ent->d_name << endl;
            }

            //write the length of the list
            write(socketfd, to_string(files.str().size()).c_str(), 255);

            //send the list of files to the client
            write(socketfd, files.str().c_str(), files.str().size());
            
        }else if(!strncmp(buff, "UPLOAD", 6)){
            //upload a file
            cout << "UPLOAD" << endl;
            
            //read the file name
            read(socketfd, buff, 255);
            cout << "Received file name : " << buff << endl;
            string file_name = string(buff);
            
            //read the file size
            read(socketfd, buff, 255);
            cout << "Received file size : " << buff << endl;

            //read the file content
            int size = atoi(buff);
            char *file_content = new char[size];
            read(socketfd, file_content, size);
            cout << "Received file content : " << file_content << endl;

            //write the file
            FILE *file = fopen(file_name.c_str(), "w");
            fwrite(file_content, 1, size, file);
            fclose(file);
            delete[] file_content;
            
        }else if(!strncmp(buff, "DOWNLOAD", 8)){
            //download a file
            cout << "DOWNLOAD" << endl;

            //read the file name
            read(socketfd, buff, 255);
            cout << "Received file name : " << buff << endl;
            string file_name = string(buff);

            //read the file content
            FILE *file = fopen(file_name.c_str(), "r");
            //go to the end of the file to get the size
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            //go back to the beginning of the file
            fseek(file, 0, SEEK_SET);
            char *file_content = new char[size];
            fread(file_content, 1, size, file);
            fclose(file);
            
            //write the file size
            write(socketfd, to_string(size).c_str(), 255);

            //send the file content
            write(socketfd, file_content, size);
        }else if(!strncmp(buff, "QUIT", 4)){
            //quit
            //cout << "QUIT" << endl;
            break;
        }else{
            //cout << "usage : LIST | UPLOAD [file] | DOWNLOAD [file] | QUIT" << endl ;
        }
    }
    //close the socket
    close(socketfd);
    //close the directory 
    closedir(dir);
}

void pr::FTPHandlerClient::handleConnection(Socket s){
    char buff[256];
    int socketfd = s.getFD();
    
    while(fgets(buff, 255, stdin)){

        if(!strncmp(buff, "LIST", 4)){
            //write the command to the server
            write(socketfd, buff, 255);
            //get the length of the list
            read(socketfd, buff, 255);
            int size = atoi(buff);
            //get the list of files
            read(socketfd, buff, size);
            cout << "Files : " << endl << buff << endl;
            
        }else if(!strncmp(buff, "UPLOAD", 6)){
            //same code as serverside download 

            //write the command to the server
            write(socketfd, buff, 255);
            //read the file name
            cout << "Enter the file name : " ;
            fgets(buff, 255, stdin);
            write(socketfd, buff, 255);
            //read the file size
            FILE *file = fopen(buff, "r");
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            fseek(file, 0, SEEK_SET);
            write(socketfd, to_string(size).c_str(), 255);

            //read the file content
            char *file_content = new char[size];
            fread(file_content, 1, size, file);
            //write the file content in the socket
            write(socketfd, file_content, size);
            fclose(file);
            
        }else if(!strncmp(buff, "DOWNLOAD", 8)){
            //same code as serverside upload

            //write the command to the server
            write(socketfd, buff, 255);
            //read the file name
            cout << "Enter the file name : " ;
            fgets(buff, 255, stdin);
            write(socketfd, buff, 255);
            //read the file size
            read(socketfd, buff, 255);
            int size = atoi(buff);
            //read the file content
            char *file_content = new char[size];
            read(socketfd, file_content, size);

            //write the file
            FILE *file = fopen(buff, "w");
            fwrite(file_content, 1, size, file);
            fclose(file);

        }else if(!strncmp(buff, "QUIT", 4)){
            //write the command to the server
            write(socketfd, buff, 255);
            break;
        }else{
            cout << "usage : LIST | UPLOAD [file] | DOWNLOAD [file] | QUIT" << endl ;   
        }
    }
}

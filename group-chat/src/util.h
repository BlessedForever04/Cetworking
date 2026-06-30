#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "model.h"

struct clientList clientList = {NULL, 0, 0};

int createTCPIpv4Socket(){
    return socket(AF_INET, SOCK_STREAM, 0); // domain : AF_INET (IP4), type : SOCK_STREAM (TCP), protocol : 0 (Default for TCP)
}

void removeClientFromClientList(int clientFD){
    for(int i = 0; i < clientList.size; i++){
        if(clientList.clients[i].clientFD == clientFD){
            clientList.clients[i] = clientList.clients[--clientList.size];
            break;
        }
    }
}

void *receiveAndBroadcastIncomingData(void *arg){
    int socketFD = *(int*)arg;
    struct response response;
    
    while(1){
        int byteReceived = recv(socketFD, &response, sizeof(struct response), 0);
        if(byteReceived > 0){
            if(strcmp(response.message, "bye\n") == 0){
                printf("%s left the server\n", response.sender); 

                struct response serverNotice;
                snprintf(serverNotice.sender, 7, "%s", "Server");
                snprintf(serverNotice.message, sizeof(response.sender), "%s", response.sender);
                strcat(serverNotice.message, " left the chat.\n");

                for(int i = 0; i < clientList.size; i++){
                    if(clientList.clients[i].clientFD != socketFD){
                        send(clientList.clients->clientFD, &serverNotice, sizeof(serverNotice), 0);
                    }
                }
                removeClientFromClientList(socketFD);
                break;
            }
            else{                
                response.message[byteReceived] = '\0';
                
                for(int i = 0; i < clientList.size; i++){
                    if(clientList.clients[i].clientFD == socketFD) continue;
                    send(clientList.clients[i].clientFD, &response, sizeof(struct response), 0);
                }
            }
        }
    }
    close(socketFD);
    return NULL;
}

void addClientToClientList(char *name, int clientFD){
    if(clientList.size == clientList.capacity){
        if(clientList.capacity == 0) clientList.capacity = 1;
        clientList.capacity = clientList.capacity * 2;
        clientList.clients = realloc(clientList.clients, sizeof(struct client) * clientList.capacity);
    }
    strcpy(clientList.clients[clientList.size++].name, name);
    clientList.clients[clientList.size-1].clientFD = clientFD;
}


void *receiveAndPrintDataFromServer(void *arg){
    int serverSocketFD = *(int*)arg;
    struct response response;
    while(1){
        int byteReceived = recv(serverSocketFD, &response, sizeof(struct response), 0);
        if(byteReceived > 0){
            response.message[byteReceived] = '\0';
            printf("%s: ", response.sender);
            printf("%s", response.message);
        }
    }
    return NULL;
}

void receivingAndBroadcastIncomingDataOnSaperateThread(int clientFD){
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, receiveAndBroadcastIncomingData, &clientFD);
}

void startAcceptingIncomingConnection(int serverSocketFD){
    while(1){
        struct acceptedConnection *acceptedClient = acceptIncomingConnection(serverSocketFD);
        char name[50];
        recv(acceptedClient->FD, name, 50, 0);
        addClientToClientList(name, acceptedClient->FD);
        receivingAndBroadcastIncomingDataOnSaperateThread(acceptedClient->FD);
    }
}
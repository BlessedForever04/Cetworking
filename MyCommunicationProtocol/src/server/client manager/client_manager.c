#include "client_manager.h"
#include <stdlib.h>
#include <string.h>

struct clientList clientList = {NULL, 0, 0};

void removeClientFromClientList(int clientFD){
    for(int i = 0; i < clientList.size; i++){
        if(clientList.clients[i].clientFD == clientFD){
            clientList.clients[i] = clientList.clients[--clientList.size];
            break;
        }
    }
}

void addClientToClientList(int clientFD){
    char name[50];
    ssize_t byteReceived = recv(clientFD, name, sizeof(name) - 1, 0);
    if(byteReceived <= 0){
        return;
    }
    name[byteReceived] = '\0';

    if(clientList.size == clientList.capacity){
        if(clientList.capacity == 0) clientList.capacity = 1;
        clientList.capacity = clientList.capacity * 2;
        clientList.clients = realloc(clientList.clients, sizeof(struct client) * clientList.capacity);
    }
    clientList.clients[clientList.size].name = malloc(strlen(name) + 1);
    strcpy(clientList.clients[clientList.size].name, name);
    clientList.clients[clientList.size].clientFD = clientFD;
    clientList.size++;
}
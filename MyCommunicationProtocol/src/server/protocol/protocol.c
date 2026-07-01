#include "../../model.h"
#include <stdio.h>
#include "protocol.h"
#include "../client manager/client_manager.h"

void manageServerProtocol(struct packetHeader header, int socketFD){
    switch (header.type){
    case PACKET_BROADCAST:
        manageBroadcast(socketFD, header);
        break;
    case PACKET_NOTICE:
        manageNotice();
        break;
    
    default:
        break;
    }
}

void manageNotice(void){
}

void manageBroadcast(int socketFD, struct packetHeader header){
    char *message = malloc(header.payloadSize);
    ssize_t byteReceived = recv(socketFD, message, header.payloadSize, 0);
    
    if(byteReceived > 0){
        for(int i = 0; i < clientList.size; i++){
            if(clientList.clients[i].clientFD != socketFD){
                send(clientList.clients[i].clientFD, &header, sizeof(header), 0);
                send(clientList.clients[i].clientFD, message, header.payloadSize, 0);
            }
        }
    }

    free(message);
}
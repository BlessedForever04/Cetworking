#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>

struct acceptedSocket{
    int FD;
    struct sockaddr_in socketAddress;
    bool acceptedSuccessfuly;
    int error;
};

int createTCPIpv4Socket(){
    return socket(AF_INET, SOCK_STREAM, 0); // domain : AF_INET (IP4), type : SOCK_STREAM (TCP), protocol : 0 (Default for TCP)
}

struct sockaddr_in* createSocketAddress(char *ip_address, uint16_t port){
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &address->sin_addr.s_addr);
    return address;
}

struct acceptedSocket* acceptIncomingConnection(int serverSocketFD){
    struct acceptedSocket *acceptedClient = malloc(sizeof(struct acceptedSocket));

    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(struct sockaddr_in);

    acceptedClient->FD = accept(serverSocketFD, (struct sockaddr*) &clientAddress, &clientAddressSize);
    acceptedClient->socketAddress = clientAddress;
    acceptedClient->acceptedSuccessfuly = (acceptedClient->FD>0);
    
    if(!acceptedClient->acceptedSuccessfuly){
        acceptedClient->error = acceptedClient->FD;
    }

    return acceptedClient;
}

void receiveAndPrintIncomingData(int serverSocketFD){
    char response[1024];

    while(1){
        int byteReceived = recv(serverSocketFD, response, 1024, 0);
        if(byteReceived > 0){
            if(strcmp(response, "bye\n") == 0){
                break;
            }
            response[byteReceived] = '\0';
            printf("From client: %s", response);
        }
    }
}
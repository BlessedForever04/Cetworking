#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"
#include <string.h>

int main(){
    uint16_t port = 5000;
    int clientSocketFD = createTCPIpv4Socket(); 
    struct sockaddr_in *serverAddressPtr = createSocketAddress("192.168.1.9", port); 

    int connectStatus = connect(clientSocketFD, (struct sockaddr*)serverAddressPtr, sizeof(*serverAddressPtr)); 
    if(connectStatus == 0) printf("Connection was successful!\n");
    if(connectStatus != 0){
        perror("connect");
        return 1;
    }

    char *message = NULL;
    size_t lineSize = 0;

    while(1){
        ssize_t charCount = getline(&message, &lineSize, stdin); 
        send(clientSocketFD, message, 1024, 0);
        if(strcmp(message, "bye\n") == 0){
            break;
        }
    }

    printf("User left the chat\n");

    close(clientSocketFD);
    return 0;
}
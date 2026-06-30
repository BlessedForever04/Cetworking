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
    
    char *name = NULL;
    printf("Enter your name: ");
    size_t lineSize = 0;
    ssize_t charCount = getline(&name, &lineSize, stdin);
    if(charCount == -1){
        perror("getline");
        free(name);
        return 1;
    }
    name[charCount-1] = '\0';

    int clientSocketFD = createTCPIpv4Socket(); 
    struct sockaddr_in *serverAddressPtr = createSocketAddress("127.0.0.1", port); 

    int connectStatus = connect(clientSocketFD, (struct sockaddr*)serverAddressPtr, sizeof(*serverAddressPtr)); 
    if(connectStatus == 0) printf("Connection was successful!\n");
    if(connectStatus != 0){
        perror("connect");
        return 1;
    }

    send(clientSocketFD, name, strlen(name) + 1, 0);

    char *message = NULL;
    
    pthread_t receiveThread;
    pthread_create(&receiveThread, NULL, receiveAndPrintDataFromServer, &clientSocketFD);
    
    while(1){
        printf("You: ");
        charCount = getline(&message, &lineSize, stdin);
        struct response response = {0};
        snprintf(response.sender, sizeof(response.sender), "%s", name);
        snprintf(response.message, sizeof(response.message), "%s", message);

        send(clientSocketFD, &response, sizeof(struct response), 0);

        if(strcmp(message, "bye\n") == 0){
            break;
        }
    }

    free(name);
    free(message);
    close(clientSocketFD);
    return 0;
}
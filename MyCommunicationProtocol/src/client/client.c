#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "../server/network/network.h"
#include "../model.h"




/*
  There are so many messed up things that has to be fixed, 
  Main this is that we can't transfer a struct directly which includes pointers as properties, 
  For that we have to send each pointer individually with its payload size
  Now this has to be done using sendAll and recvAll implementation

  Next thing to do is that we have to implement different protocols for server and client
  header management has to be done in this phase
*/







int main(){
    uint16_t port = 5000;

    // Client's name
    char *name = NULL;
    printf("Enter your name: ");
    size_t lineSize = 0;
    ssize_t charCount = getline(&name, &lineSize, stdin);
    lineSize = 0;
    
    if(charCount == -1){
        perror("getline");
        free(name);
        return 1;
    }
    
    name[charCount-1] = '\0';

    // Socket and address creation
    int socketFD = createTCPIpv4Socket(); 
    struct sockaddr_in *serverAddressPtr = createSocketAddress("127.0.0.1", port); 
    
    // Connecting to server socket
    int connectStatus = connect(socketFD, (struct sockaddr*)serverAddressPtr, sizeof(*serverAddressPtr)); 
    if(connectStatus == 0) printf("Connection was successful!\n");
    if(connectStatus != 0){
        perror("connect");
        return 1;
    }
    
    // Sending client name to server
    send(socketFD, name, strlen(name) + 1, 0);    
    // Thread for receiving data from server and printing on terminal
    pthread_t receiveThread;
    pthread_create(&receiveThread, NULL, receiveDataFromServer, &socketFD);
    
    // Writing message to server
    struct messagePacket message;
    message.sender = malloc(strlen(name) + 1);
    while(1){
        charCount = getline(&message.message, &lineSize, stdin);
        snprintf(message.sender, strlen(name) + 1, "%s", name);
        
        struct packetHeader header = {0};
        if(strcmp(message.message, "bye\n") == 0){
            snprintf(message.sender, 7, "%s", "Server");
            snprintf(message.message, lineSize, "%s left the chat\n", name);

            header.payloadSize = strlen(message.message) + 1;
            header.type = PACKET_USER_LEFT;

            send(socketFD, &header, sizeof(header), 0); // Header
            send(socketFD, message.message, header.payloadSize, 0); // Message payload

            break;
        }

        header.payloadSize = strlen(message.message) + 1;
        header.type = PACKET_BROADCAST;

        // Sending packet header
        send(socketFD, &header, sizeof(header), 0);
        send(socketFD, message.message, header.payloadSize, 0);
    }

    free(name);
    free(message.message);
    free(message.sender);
    close(socketFD);
    return 0;
}
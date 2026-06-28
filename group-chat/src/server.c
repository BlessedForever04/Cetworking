#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "util.h"
#include <unistd.h>

int main(){
    uint16_t port = 5000;
    // Creating socket and address
    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in *serverAddressPtr = createSocketAddress("192.168.1.9", port);

    // Binding the address to socket (AKA assigning name to socket)
    int bindStatus = bind(serverSocketFD, (struct sockaddr*) serverAddressPtr, sizeof(*serverAddressPtr));
    if(bindStatus == 0) printf("Server binding is successful!\n");
    if(bindStatus < 0){
        perror("bind");
        return 1;
    }

    int listenStatus = listen(serverSocketFD, 5);
    if(listenStatus == 0) printf("Server is listening on port %d..\n", port);
    if(listenStatus != 0){
        perror("listen");
        return 1;
    }

    struct sockaddr_in *clientAddressPtr;
    socklen_t clientAddressSize = sizeof(struct sockaddr_in);

    int clientSocketFD = accept(serverSocketFD,(struct sockaddr*) clientAddressPtr, &clientAddressSize);

    char response[1024];

    while(1){
        printf("Listening to client..\n");
        int bytesReceived = recv(clientSocketFD, response, 1024, 0);
        printf("Heard something..\n");
        if(bytesReceived > 0){
            response[bytesReceived] = '\0';
            printf("From client: %s", response);
        }
    }

    close(clientSocketFD);
    close(serverSocketFD);

    return 0;
}
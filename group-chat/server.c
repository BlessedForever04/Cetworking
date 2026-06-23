#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "util.h"
#include <unistd.h>

int main(){
    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in *serverAddressPtr = createSocketAddress("192.168.1.8", 5000);

    int bindStatus = bind(serverSocketFD, (struct sockaddr*) serverAddressPtr, sizeof(*serverAddressPtr));
    if(bindStatus == 0) printf("Server binding is successful!\n");
    if(bindStatus < 0){
        printf("Binding failed!\n");
        return 1;
    }

    int listenStatus = listen(serverSocketFD, 5);
    if(listenStatus == 0) printf("Server is listening..\n");

    struct sockaddr_in *clientAddressPtr;
    socklen_t clientAddressSize = sizeof(struct sockaddr_in);

    int clientSocketFD = accept(serverSocketFD,(struct sockaddr*) clientAddressPtr, &clientAddressSize);

    char response[1024];

    int bytesReceived = recv(clientSocketFD, response, 1024, 0);

    if(bytesReceived > 0){
        response[bytesReceived] = '\0';
        printf("From client: %s", response);
    }

    close(clientSocketFD);
    close(serverSocketFD);

    return 0;
}
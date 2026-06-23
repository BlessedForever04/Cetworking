#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util.h"

int main(){
    int clientSocketFD = createTCPIpv4Socket(); 
    struct sockaddr_in *serverAddressPtr = createSocketAddress("192.168.1.8", 5000); 

    int result = connect(clientSocketFD, (struct sockaddr*)serverAddressPtr, sizeof(*serverAddressPtr)); 
    if(result == 0) printf("Connection was successful!\n");

    char *message = "Ji Ji\n";

    send(clientSocketFD, message, 1024, 0);
    return 0;
}
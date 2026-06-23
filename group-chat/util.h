#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

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
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "../../model.h"

void manageServerProtocol(struct packetHeader header, int socketFD);
void manageNotice(void);
void manageBroadcast(int socketFD, struct packetHeader header);

#endif
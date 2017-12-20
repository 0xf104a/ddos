//
//  socket.h
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#ifndef socket_h
#define socket_h

#include "message.h"
#include <arpa/inet.h>
#include <errno.h> //For errno - the error number
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

extern bool socket_wait;

int dos_tcp_sock(char* host, int port);
char* dos_tcp_send(int sock, char* data);
int dos_udp_sock(void);
char* dos_udp_send(int sock, char* host, int port, char* message);
int hostname2ip(char* hostname, char* ip);
#endif /* socket_h */

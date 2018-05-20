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
#include "packet.h"

#include <arpa/inet.h>
#include <errno.h> //For errno - the error number
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

extern bool socket_wait;

int dos_tcp_sock(char* host, int port);
void dos_tcp_send_no_recv(int sock, char* data);
bool dos_tcp_send_noalloc(int sock, char* data, char* buf, size_t bufsize);
char* dos_tcp_send(int sock, char* data);
int dos_udp_sock(void);
bool dos_udp_send(int sock, char* host, int port, char* message, char* buf, size_t bufsize);
ssize_t dos_raw_send(int sock,void* __data,uint16_t port,char* target,size_t len);
int dos_raw_sock(int _proto,bool is_headers_raw);
int hostname2ip(const char* hostname, char* ip);
#endif /* socket_h */

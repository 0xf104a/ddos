//
//  ddos.h
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#ifndef ddos_h
#define ddos_h

#include "message.h"
#include "socket.h"
#include "util.h"

#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MODE_EMPTY 0 //not initiallized mode
#define MODE_TCP 1
#define MODE_UDP 2
#define MODE_ICMP 3 //For future releases
#define MODE_SYNFLOOD 4
#define MODE_MEMCRASHED 5
#define MODE_UDPV2 6

extern double psent;
extern uint8_t metrics;
extern bool status;//is status shown?
extern int tcount;
extern char* __host;
extern int __port;
extern pthread_mutex_t mutex;
extern bool use_dos_sleep;
extern int dos_sleep;
extern bool __run;

typedef struct {
    char* packet;
    char* host;
    int port;
    uint8_t mode;
} _dos_param;

void ddos(char* host, int port, char* packet, int _tcount, int mode);

#endif /* ddos_h */

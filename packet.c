//
//  ip.c
//  cdos
//
//  Created by Andre Zay on 07.05.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#include "packet.h"
#include "message.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

static uint16_t _checksum(void* vdata,size_t length) {
    // Cast the data pointer to one that can be indexed.
    char* data=(char*)vdata;
    
    // Initialise the accumulator.
    uint32_t acc=0xffff;
    
    // Handle complete 16-bit blocks.
    for (size_t i=0;i+1<length;i+=2) {
        uint16_t word;
        memcpy(&word,data+i,2);
        acc+=ntohs(word);
        if (acc>0xffff) {
            acc-=0xffff;
        }
    }
    
    // Handle any partial block at the end of the data.
    if (length&1) {
        uint16_t word=0;
        memcpy(&word,data+length-1,1);
        acc+=ntohs(word);
        if (acc>0xffff) {
            acc-=0xffff;
        }
    }
    
    // Return the checksum in network byte order.
    return htons(~acc);
}

ipheader* make_iphdr(uint8_t ttl,uint8_t proto,uint32_t src,uint32_t dst,uint16_t id,uint8_t tos,uint16_t len){
    ipheader* x=(ipheader*)malloc(sizeof(ipheader));
    memset(x,0,(size_t)sizeof(ipheader));
    x->iph_ihl=5;
    x->iph_ver=4;
    //x->iph_len=sizeof(ipheader);
    x->iph_chksum=0;
    x->iph_protocol=proto;
    x->iph_len=len;
    x->iph_ttl=ttl;
    x->iph_sourceip=src;
    x->iph_destip=dst;
    x->iph_tos=tos;
    x->iph_ident=htons(id);
    return x;
}

ipheader* make_iphcommon(char *src,char* dst,uint8_t proto,uint16_t len){
    return make_iphdr(64, proto, inet_addr(src), inet_addr(dst), 54321, 0xf,len);
}

udpheader* make_udphdr(uint16_t srcport,uint16_t dstport,uint16_t len){
    udpheader* x=(udpheader*)malloc(sizeof(udpheader));
    memset(x, 0, sizeof(udpheader));
    x->udph_srcport=srcport;
    x->udph_destport=dstport;
    x->udph_srcport=srcport;
    return x;
}

udppacket* setup_udppacket(char* src,char* dst,uint16_t srcport,uint16_t dstport,char* payload){
    udppacket* x=(udppacket*)malloc(sizeof(udppacket));
    x->ip=make_iphcommon(src, dst, IPPROTO_UDP,strlen(payload));
    x->udp=make_udphdr(srcport, dstport, htons(sizeof(udpheader)+strlen(payload)*sizeof(char)));
    x->payload=(char*)malloc(strlen(payload)*sizeof(char));
    strcpy(x->payload, payload);
    x->payload_size=strlen(payload);
    x->__sz=sizeof(ipheader)+sizeof(udpheader)+x->payload_size;
    return x;
}
char* __packudp(udppacket* packet){
    char* buffer=(char*)malloc(packet->__sz);
    info("Allocated %d bytes of memory",packet->__sz);
    memset(buffer, 0, packet->__sz);
    strcat(buffer,(char*)packet->ip);
    strcat(buffer,(char*)packet->udp);
    strcat(buffer, packet->payload);
    return buffer;
}
char* udppacket_pack(udppacket* packet){
    char* buffer=__packudp(packet);
    uint16_t chk=_checksum(buffer, sizeof(ipheader)+sizeof(udpheader));
    packet->ip->iph_chksum=chk;
    buffer=__packudp(packet);
    return buffer;
}





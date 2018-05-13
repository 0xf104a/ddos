//
//  ip.c
//  cdos
//
//  Created by Andre Zay on 07.05.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#include "packet.h"
#include <stdlib.h>
#include <stdint.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>

static unsigned short csum(unsigned short *buf, register int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--){
        sum += *buf++;
    }
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

ipheader* make_iphdr(uint8_t ttl,uint8_t proto,uint32_t src,uint32_t dst,uint16_t id,uint8_t tos){
    ipheader* x=(ipheader*)malloc(sizeof(ipheader));
    memset(x,0,(size_t)sizeof(ipheader));
    x->iph_ihl=5;
    x->iph_ver=4;
    x->iph_len=sizeof(ipheader);
    x->iph_chksum=0;
    x->iph_protocol=proto;
    x->iph_ttl=ttl;
    x->iph_sourceip=src;
    x->iph_destip=dst;
    x->iph_tos=tos;
    x->iph_ident=htons(id);
    return x;
}

ipheader* make_iphcommon(char *src,char* dst,uint8_t proto){
    return make_iphdr(64, proto, inet_addr(src), inet_addr(dst), 54321, IPTOS_LOWDELAY);
}

//TODD:udpheader* make_udphdr


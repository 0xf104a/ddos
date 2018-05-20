//
//  ip.h
//  cdos
//
//  Created by Andre Zay on 07.05.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#ifndef packet_h
#define packet_h

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct _ipheader {
    unsigned char      iph_ihl:5, iph_ver:4;
    unsigned char      iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned char      iph_flag;
    unsigned short int iph_offset;
    unsigned char      iph_ttl;
    unsigned char      iph_protocol;
    unsigned short int iph_chksum;
    unsigned int       iph_sourceip;
    unsigned int       iph_destip;
} ipheader;

typedef struct _udpheader {
    unsigned short int udph_srcport;
    unsigned short int udph_destport;
    unsigned short int udph_len;
    unsigned short int udph_chksum;
} udpheader;

typedef struct _udppacket{
    ipheader*  ip;
    udpheader* udp;
    char*      payload;
    size_t     payload_size;
    size_t     __sz;//Full packet size
}udppacket;

udppacket* setup_udppacket(char* src,char* dst,uint16_t srcport,uint16_t dstport,char* payload);
udpheader* make_udphdr(uint16_t srcport,uint16_t dstport,uint16_t len);
ipheader* make_iphdr(uint8_t ttl,uint8_t proto,uint32_t src,uint32_t dst,uint16_t id,uint8_t tos,uint16_t len);
ipheader* make_iphcommon(char *src,char* dst,uint8_t proto,uint16_t len);
char* udppacket_pack(udppacket* packet);
#endif /* packet_h */

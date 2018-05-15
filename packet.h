//
//  ip.h
//  cdos
//
//  Created by Andre Zay on 07.05.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#ifndef ip_h
#define ip_h

#include <stddef.h>

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


#endif /* ip_h */

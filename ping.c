//
//  ping.c
//  cdos
//
//  Created by Andre Zay on 22.04.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

//icmp ping implementaion

#include "ping.h"
#include "message.h"

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/signal.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip_var.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

char *sendpacket;

unsigned short chksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;
    
    for ( sum = 0; len > 1; len -= 2 ){
        sum += *buf++;
    }
    if ( len == 1 ){
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

struct icmp* pack(int pack_no,int32_t datalen)

{
    int packsize;
    struct icmp *icmp;
    struct timeval *tval;
    //Inittiallize icmp packet
    icmp = (struct icmp*)sendpacket;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq = pack_no;
    icmp->icmp_id = 0;
    packsize = 8+datalen;
    tval = (struct timeval*)icmp->icmp_data;
    gettimeofday(tval, NULL);
    icmp->icmp_cksum = chksum((unsigned short*)icmp, packsize);
    return icmp;
}

bool _ping(int sockfd,struct sockaddr_in dest_addr,int _psize){//ping once
    int packetsize=8+_psize;
    int n, fromlen;
    int _errno;
    struct sockaddr_in from;
    fromlen = sizeof(from);
    if(sendto(sockfd, sendpacket, packetsize, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr))<0){
        warning("sendto() failed!(%s:%d)",__FILE__,__LINE__);
        return false;
    }
    //Now receive answer back
    char *buf;
    const socklen_t slen=sizeof(dest_addr);
    recvfrom(sockfd, buf, _psize, MSG_PEEK, (struct sockaddr*)&dest_addr, &slen);
}
bool ping(struct sockaddr_in *addr,uint32_t trycount,int32_t _psize)
{
    /*
     *ping -- ping check whether host is up
     *addr -- host address
     *trycount--number of tries in case of host isn't answering
     *psize--packet size
     */
    int i=0;
    while(i<trycount){
        
    }

}

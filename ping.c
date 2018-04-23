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
#include "socket.h"//gethostbyname

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
#include <stdlib.h>

char *sendpacket;
int ping_timeout;//TODO:Add optional timeout
//FIXME:add checksum and make it fully workable ping(as described in RFC)

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
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    char *recvpacket = (char*)malloc(sizeof(char)*4096);
    int code=sendto(sockfd, sendpacket, packetsize, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    errno_t error=errno;
    if(code<0){
        char *errorbuf = strerror(error);
        warning("sendto() failed:%s(%s:%d)",errorbuf,__FILE__,__LINE__);
        return false;
    }
    //Now receive answer back
    if(recvfrom(sockfd, recvpacket, 4096, 0, (struct sockaddr*) &from, &fromlen) < 0){
        return true;
    }else{
        warning("Ping response wasn't received,host seems to be down");
        return false;
    }
}
bool ping(char* _host,uint32_t trycount,int32_t _psize)
{
    /*
     *ping -- ping check whether host is up
     *addr -- host address
     *trycount--number of tries in case of host isn't answering
     *psize--packet size
     */
    int i=0;
    struct hostent *host;
    struct sockaddr_in dest_addr;
    struct protoent *protocol;
    unsigned long inaddr = 0l;
    int sockfd;
    int size = 50 * 1024;
    if ((protocol = getprotobyname("icmp")) == NULL)
        
    {
        perror("getprotobyname");
        exit(-1);
    }
    sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto);
    errno_t error_num = errno;
    if (sockfd < 0)
    {
        char *errorbuf = strerror(error_num);
        die("Failed to create socket:%s(%s:%d)",errorbuf,__FILE__,__LINE__);
        exit(-1);
    }
    setuid(getuid());
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inaddr = inet_addr(_host);
    if (inaddr == INADDR_NONE)
    {
        if ((host = gethostbyname(_host)) == NULL)
        {
            die("%s:Unknown host",host);
        }
        memcpy((char*) &dest_addr.sin_addr, host->h_addr, host->h_length);
    }else{
        dest_addr.sin_addr.s_addr = inet_addr(_host);
    }
    while(i<trycount){
        if(_ping(sockfd,dest_addr,_psize)){
            return true;
        }
        i++;
    }
    return false;
}

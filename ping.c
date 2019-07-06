#include "ping.h"
#include "message.h"

#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
//Code of this module is based on https://github.com/hayderimran7/advanced-socket-programming/blob/master/ping.c
#ifdef __ANDROID__
#warning Non-rooted android devices does not support ping module
#endif

#define errno_t int

#define DEBUG

#define PACKET_SIZE     64


char sendpacket[PACKET_SIZE];
char recvpacket[PACKET_SIZE];

int sockfd, datalen = 56;
int nsend = 0;
struct sockaddr_in dest_addr;
pid_t pid;
struct sockaddr_in from;
struct timeval tvrecv;

unsigned short cal_chksum(unsigned short *addr, int len)

{
    
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;
    while (nleft > 1)
    {
        sum +=  *w++;
        nleft -= 2;
    }
    if (nleft == 1)
    {
        *(unsigned char*)(&answer) = *(unsigned char*)w;
        sum += answer;
    }
    
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    
    return answer;
    
}
int pack(int pack_no)

{
    int packsize;
    struct icmp *icmp;
    struct timeval *tval;
    icmp = (struct icmp*)sendpacket;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq = pack_no;
    icmp->icmp_id = pid;
    packsize = 8+datalen;
    tval = (struct timeval*)icmp->icmp_data;
    icmp->icmp_cksum = cal_chksum((unsigned short*)icmp, packsize);
    return packsize;
    
}
bool send_packet()
{
    int n,packetsize,fromlen;

    packetsize = pack(nsend);
    if (sendto(sockfd, sendpacket, packetsize, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0){
            error("Failed to send data to host:%s(%d)",strerror(errno),errno);
            return false;
    }
    if ((n = recvfrom(sockfd, recvpacket, sizeof(recvpacket), 0, (struct sockaddr*) &from, &fromlen) < 0)){
        errno_t err=errno;
        if (errno == EINTR){
            return false;
        }
        error("Failed to receive data from host:%s(%d)",strerror(err),err);
        return false;
    }
    return true;
}


bool ping(char *_host,uint32_t max,int _timeout)

{
    info("Checking host");
    
    struct timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    
    struct hostent *host;
    struct protoent *protocol;
    unsigned long inaddr = 0l;
    int size = 50 * 1024;
    if ((protocol = getprotobyname("icmp")) == NULL)
    {
        warning("Failed to get protocol by name(%d)",errno);
        warning("Ping check aborted");
    }
    if ((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
    {
        warning("Failed error:%d",errno);
        warning("Ping check aborted");
    }
    
    setuid(getuid());
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
    
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inaddr = inet_addr(_host);
    
    if (inaddr == INADDR_NONE)
    {
        if ((host = gethostbyname(_host)) == NULL)
        {
            perror("gethostbyname error");
            exit(1);
        }
        memcpy((char*) &dest_addr.sin_addr, host->h_addr, host->h_length);
    }else{
        dest_addr.sin_addr.s_addr = inet_addr(_host);
    }
    pid = getpid();
    int i=0;
    while(i<max){
        if(send_packet()){
            success("Host is up");
            return true;
#ifdef DEBUG
        }else{
            warning("Ping icmp seq #%d fault",i);
        }
#else
        }
#endif
        i++;
    }
    error("Maximum number of ping probes exceeded");
    return false;
}

//
// Created by Peer De bakker on 11/20/21.
//

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <ft_ping.h>

unsigned short checksum(void *b, int len)
{   unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void    send_packet(int socketFd, struct addrinfo* dst_addrinfo) {
//    socklen_t len=sizeof(struct sockaddr_in);
//    struct icmp *icmp_send,
//                *icmp_recv;
//
//    icmp_send->icmp_type = ICMP_ECHO;
//    icmp_send->icmp_code = 0;
//    icmp_send->icmp_id = getpid();
//    icmp_send->icmp_seq = htons(1);
    unsigned char   buff[200];
//    unsigned char*  p = buff;
//    p += sizeof(icmp_send);
//    *p = 'A';
//
//    icmp_send->icmp_cksum = 0;
//    memcpy(buff, icmp_send, sizeof(icmp_send)) ;
//
//    icmp_send->icmp_cksum = checksum(buff, sizeof(icmp_send) + 1);
//    memcpy(buff, icmp_send, sizeof(icmp_send)) ;

    int error = sendto(socketFd, buff, sizeof(buff), 0, (struct sockaddr*)dst_addrinfo, sizeof(struct addrinfo));
    if (error) {
        perror("sendto");
        exit_error("Error sending message");
    } else {
        printf("no error sending message\n");
    }

//    if ( sendto(sd, (unsigned char*)buff, sizeof(icmp_send) + 1, 0, (struct sockaddr*)addr, sizeof(*addr)) <= 0 ) {
//        printf("Send err.\n");
//    }
}

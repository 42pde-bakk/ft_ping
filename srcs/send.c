//
// Created by Peer De bakker on 11/20/21.
//

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ft_ping.h>


// Internet checksum (RFC 1071) for error checking, calculated from the ICMP header and data with value 0 substituted for this field.
unsigned short checksum(void *b, int len)
{   unsigned short* buf = b;
    unsigned int    sum;
    unsigned short  result;

    for ( sum = 0; len > 1; len -= 2 ) {
        sum += *buf;
        ++buf;
    }
    if ( len == 1 ) {
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}


int    send_packet(t_ping* ping) {
    char sent_package[PACKET_SIZE];

    ++ping->sequence;

    gen_ip_header(sent_package, ping->dest.sin_addr.s_addr);
    gen_icmp_msg(sent_package + IP_HDR_SIZE, ping->sequence);

    ssize_t sent_bytes = sendto(ping->socketFd, sent_package, PACKET_SIZE, 0, (struct sockaddr*)&ping->dest, sizeof(ping->dest));
    if (sent_bytes == -1) {
        perror("sendto");
        return (1);
    }
    printf("done sending packet\n");
    return (0);
}

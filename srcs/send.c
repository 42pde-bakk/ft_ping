//
// Created by Peer De bakker on 11/20/21.
//

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdio.h>
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

void    initialize_packet(struct s_packet* packet, t_ping* ping) {
    (void)ping;
    memset(packet, 0, sizeof(*packet));

    packet->icmp_header.icmp_type = ICMP_ECHO; // Echo Request
    packet->icmp_header.icmp_code = ICMP_ECHOREPLY; // Echo Reply
    packet->icmp_header.icmp_cksum = checksum(packet, sizeof(*packet));
    packet->icmp_header.icmp_id = htons(ping->pid);
    packet->icmp_header.icmp_seq = htons(0); //= ping->messageCount;
}

int    send_packet(t_ping* ping) {
    t_packet packet;
    memset(&packet, 0, sizeof(t_packet));

    initialize_packet(&packet, ping);
    ssize_t sent_bytes = sendto(ping->socketFd, &packet, sizeof(t_packet), 0, (struct sockaddr*)ping->addrInfo->ai_addr, sizeof(*ping->addrInfo->ai_addr));

    if (sent_bytes == -1) {
        perror("sendto");
        return (1);
    }
    printf("done sending packet\n");
    ping->messageCount++;
    return (0);
}

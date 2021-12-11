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
#include <arpa/inet.h>
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
    memset(ping->packet.buf, 0, PACKET_SIZE);
    ping->packet.ip->version = 4;
    ping->packet.ip->ihl = sizeof(*ping->packet.ip) >> 2;
	ping->packet.ip->protocol = IPPROTO_ICMP;
	
	inet_pton(AF_INET, ping->destination_address, ping->packet.ip->daddr);
	ping->packet.hdr->type = ICMP_ECHO;
	ping->packet.hdr->code = ICMP_ECHOREPLY;
	ping->packet.hdr->un.echo.id = getpid();
	ping->packet.hdr->un.echo.sequence = ping->sequence++;
	ping->packet.hdr->checksum = checksum((unsigned short*)ping->packet.hdr, sizeof(struct icmphdr));

	if (sendto(ping->socketFd, (void*)&ping->packet, PACKET_SIZE, 0, (void*)ping->rec_in, sizeof(struct sockaddr_in)) < 0) {
		perror("sendto");
		return (1);
	}
	return (0);
}

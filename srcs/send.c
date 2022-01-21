#include "ft_ping.h"
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>


void	send_packet(t_ping* ping)
{
    bzero((void *)ping->pckt.buf, PACKET_SIZE);
    ping->pckt.ip->version = 4;
    ping->pckt.ip->ihl = sizeof(*ping->pckt.ip) >> 2;
    ping->pckt.ip->ttl = ping->ttl;
    ping->pckt.ip->protocol = IPPROTO_ICMP;
    inet_pton(AF_INET, ping->addrstr, &ping->pckt.ip->daddr);
    ping->daddr = ping->pckt.ip->daddr;
    ping->pckt.hdr->type = ICMP_ECHO;
    ping->pckt.hdr->code = 0;
    ping->pckt.hdr->un.echo.id = ping->pid;
    ping->pckt.hdr->un.echo.sequence = ping->seq++;
    ping->pckt.hdr->checksum = checksum((unsigned short*)ping->pckt.hdr,
                                            sizeof(struct icmphdr));
    if (sendto(ping->sockfd, (void *)&ping->pckt, PACKET_SIZE, 0,
               (void *)ping->rec_in,
               sizeof(struct sockaddr_in)) < 0)
        exit_error("Error: sendto");
    if (gettimeofday(&ping->time.s, NULL) < 0)
        exit_error("Error: gettimeofday");
    ping->sent > 1 ? gettimeofday(&ping->time.time_start, NULL) : 0;
    ping->sent++;
    g_signals.send = 0;
    printf("set g_signals.send to 0\n");
}
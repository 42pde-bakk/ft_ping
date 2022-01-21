#include "ft_ping.h"
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>

void	init_packet(t_ping* ping) {
	t_pckt* pckt = &ping->pckt;
    bzero((void *)pckt->buf, PACKET_SIZE);
    pckt->ip->version = IPVERSION;
    pckt->ip->ihl = sizeof(*ping->pckt.ip) >> 2;
    pckt->ip->ttl = ping->ttl;
    pckt->ip->protocol = IPPROTO_ICMP;
    inet_pton(AF_INET, ping->addrstr, &pckt->ip->daddr);
    ping->daddr = pckt->ip->daddr;
    pckt->hdr->type = ICMP_ECHO;
    pckt->hdr->code = ICMP_ECHOREPLY;
    pckt->hdr->un.echo.id = ping->pid;
    pckt->hdr->un.echo.sequence = ping->seq++;
    pckt->hdr->checksum = checksum((unsigned short*)pckt->hdr, sizeof(struct icmphdr));
}

void	send_packet(t_ping* ping)
{
	init_packet(ping);
    if (sendto(ping->sockfd, (void *)&ping->pckt, PACKET_SIZE, 0,
               (void *)ping->rec_in, sizeof(struct sockaddr_in)) < 0) {
        exit_error("Error: sendto");
	}
	save_current_time(&ping->time.s);
    ping->sent > 1 ? gettimeofday(&ping->time.time_start, NULL) : 0;
    ping->sent++;
    g_signals.send = 0;
    printf("set g_signals.send to 0\n");
}
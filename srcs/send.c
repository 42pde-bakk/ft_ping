#include "ft_ping.h"
#include <arpa/inet.h>

void	init_packet(t_ping* ping) {
	t_pckt* pckt = &ping->pckt;

    ft_bzero((void *)pckt->buf, PACKET_SIZE);
    pckt->ip->version = IPVERSION;
    pckt->ip->ihl = sizeof(*ping->pckt.ip) >> 2;
	pckt->ip->id = ping->pid;
    pckt->ip->ttl = TTL;
    pckt->ip->protocol = IPPROTO_ICMP;
	pckt->ip->tot_len = sizeof(pckt->buf);
    if (inet_pton(AF_INET, ping->addrstr, &pckt->ip->daddr) == -1)
		exit_error("Error: inet_pton");
    ping->daddr = pckt->ip->daddr;
    pckt->hdr->type = ICMP_ECHO;
    pckt->hdr->code = ICMP_ECHOREPLY;
    pckt->hdr->un.echo.id = ping->pid;
    pckt->hdr->un.echo.sequence = ping->seq++;
    pckt->hdr->checksum = 0;
    pckt->hdr->checksum = checksum((unsigned short*)pckt->hdr, sizeof(struct icmphdr), 0);
}

void send_packet(t_ping *ping, t_time *time)
{
	init_packet(ping);
	ssize_t ret = sendto(ping->sockfd, (void *)&ping->pckt, PACKET_SIZE, 0,
                         (void *)ping->rec_in, sizeof(struct sockaddr_in));
    if (ret < 0) {
        exit_error("Error: sendto");
	}
	save_current_time(&time->s);
	if (ping->seq - 1 == 0) {
		save_current_time(&time->time_start);
		save_current_time(&time->r);
	}
    g_signals.send = 0;
}

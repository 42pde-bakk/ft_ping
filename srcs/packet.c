//
// Created by Peer De bakker on 1/19/22.
//

#include "ft_ping.h"
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>

void	send_packet(t_ping* ping)
{
    bzero((void *)ping->pckt.buf, PACKET_PING_SIZE);
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
    if (sendto(ping->sockfd, (void *)&ping->pckt, PACKET_PING_SIZE, 0,
               (void *)ping->rec_in,
               sizeof(struct sockaddr_in)) < 0)
        exit_error("Error: sendto");
    if (gettimeofday(&ping->time.s, NULL) < 0)
        exit_error("Error: gettimeofday");
    ping->sent > 1 ? gettimeofday(&ping->time.time_start, NULL) : 0;
    ping->sent++;
    g_signals.send = 0;
}

void	calc_rtt(t_ping* ping)
{
    long double rtt;

    if (gettimeofday(&ping->time.r, NULL) < 0)
        exit_error("Error: gettimeofday");
    ping->received++;
    rtt = (ping->time.r.tv_usec - ping->time.s.tv_usec) / 1000000.0;
    rtt += (ping->time.r.tv_sec - ping->time.s.tv_sec);
    rtt *= 1000.0;
    ping->time.rtt = rtt;
    if (rtt > ping->time.max)
        ping->time.max = rtt;
    if (rtt < ping->time.min || ping->time.min == 0.0)
        ping->time.min = rtt;
    ping->time.avg += rtt;
    ping->time.sum_square += rtt * rtt;
}

void	init_header(t_ping* ping)
{
    t_res	*res;

    res = &ping->response;
    bzero((void *)ping->pckt.buf, PACKET_PING_SIZE);
    bzero(res, sizeof(t_res));
    res->iov->iov_base = (void *)ping->pckt.buf;
    res->iov->iov_len = sizeof(ping->pckt.buf);
    res->msg.msg_iov = res->iov;
    res->msg.msg_iovlen = 1;
    res->msg.msg_name = NULL;
    res->msg.msg_namelen = 0;
    res->msg.msg_flags = MSG_DONTWAIT;
}

void	printf_v(t_ping* ping)
{
    char		str[50];

    printf("%d bytes from %s: type=%d code=%d\n",
           ping->bytes - (int)sizeof(struct iphdr),
           inet_ntop(AF_INET, (void*)&ping->pckt.ip->saddr, str, 100),
           ping->pckt.hdr->type, ping->pckt.hdr->code);
}

void	get_packet(t_ping* ping)
{
    int			ret;

    init_header(ping);
    while (!g_signals.finito)
    {
        ret = recvmsg(ping->sockfd, &ping->response.msg, MSG_DONTWAIT);
        if (ret > 0)
        {
            ping->bytes = ret;
            if (ping->pckt.hdr->un.echo.id == ping->pid)
            {
                calc_rtt(ping);
                printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2Lf ms\n",
                       ping->bytes - (int)sizeof(struct iphdr), ping->addrstr,
                       ping->pckt.hdr->un.echo.sequence, ping->pckt.ip->ttl,
                       ping->time.rtt);
            }
            else if (ping->flags & FLAG_V)
                printf_v(ping);
            return ;
        }
    }
}

//
// Created by Peer De bakker on 1/19/22.
//

#include "ft_ping.h"
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>

void	init_header(t_res* res, t_ping* ping)
{
    bzero(res, sizeof(t_res));
    bzero((void *)ping->pckt.buf, PACKET_SIZE);

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

	bzero(str, sizeof(str));
    printf("%d bytes from %s: type=%d code=%d\n",
           ping->bytes - (int)sizeof(struct iphdr),
           inet_ntop(AF_INET, (void*)&ping->pckt.ip->saddr, str, 100),
           ping->pckt.hdr->type, ping->pckt.hdr->code);
}

void	get_packet(t_ping* ping)
{
	t_res	response;
    int		ret = 0;

    init_header(&response, ping);
    while (!g_signals.finito)
    {
        ret = recvmsg(ping->sockfd, &response.msg, MSG_DONTWAIT);
        if (ret > 0)
        {
            ping->bytes = ret;
            if (ping->pckt.hdr->un.echo.id == ping->pid)
            {
                calc_rtt(ping);
                printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.2Lf ms\n",
                       ping->bytes - sizeof(struct iphdr), ping->addrstr,
                       ping->pckt.hdr->un.echo.sequence, ping->pckt.ip->ttl,
                       ping->time.rtt);
            }
            else if (ping->flags & FLAG_V)
                printf_v(ping);
            return ;
        }
    }
}

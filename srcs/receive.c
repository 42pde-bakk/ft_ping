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

void	printf_v(ssize_t ret, t_ping* ping)
{
    char		str[50];

	bzero(str, sizeof(str));
	(void)ret;
	(void)ping;
    // printf("%lz bytes from %s: type=%d code=%d\n",
    //        ret - sizeof(struct iphdr),
    //        inet_ntop(AF_INET, (void*)&ping->pckt.ip->saddr, str, 100),
    //        ping->pckt.hdr->type, ping->pckt.hdr->code);
}

void	display_receive_msg(ssize_t ret, t_ping* ping) {
	printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.2Lf ms\n",
		ret - sizeof(struct iphdr),
		ping->addrstr,
		ping->pckt.hdr->un.echo.sequence,
		ping->pckt.ip->ttl,
		ping->time.rtt
	);
}

void	get_packet(t_ping* ping) {
	t_res	response;
    ssize_t		ret;

    init_header(&response, ping);
    while (g_signals.running)
    {
        ret = recvmsg(ping->sockfd, &response.msg, MSG_DONTWAIT);
        if (ret > 0)
        {
            printf("received packet with hdr->un.echo.id=%d\n", ping->pckt.hdr->un.echo.id);
            if (ping->pckt.hdr->un.echo.id == ping->pid)
            {
                calc_rtt(ping);
				display_receive_msg(ret, ping);
            } else { // else if (ping->flags & FLAG_V)
				printf_v(ret, ping);
			}
            break;
        }
    }
}

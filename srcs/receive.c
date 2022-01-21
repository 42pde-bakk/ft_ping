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

void	get_packet(t_ping* ping) {
	t_res	response;
    ssize_t		ret;

    init_header(&response, ping);
    while (g_signals.running)
    {
        ret = recvmsg(ping->sockfd, &response.msg, MSG_DONTWAIT);
        if (ret > 0)
        {
			printf("r3.ping->pckt->ip->ttl = %d\n", ping->pckt.ip->ttl);
            printf("received packet with hdr->un.echo.id=%d\n", ping->pckt.hdr->un.echo.id);
            if (ping->pckt.hdr->un.echo.id == ping->pid)
            {
                double rtt = calc_rtt(ping);
				display_receive_msg(ret, ping, rtt);
	            break;
            } else { // else if (ping->flags & FLAG_V)
				display_receive_msg_v(ret, ping);
			}
        }
    }
}

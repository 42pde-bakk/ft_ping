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
    ft_bzero(res, sizeof(t_res));
    ft_bzero((void *)ping->pckt.buf, PACKET_SIZE);

    res->iov->iov_base = (void *)ping->pckt.buf;
    res->iov->iov_len = sizeof(ping->pckt.buf);
    res->msg.msg_iov = res->iov;
    res->msg.msg_iovlen = 1;
    res->msg.msg_name = NULL;
    res->msg.msg_namelen = 0;
    res->msg.msg_flags = MSG_DONTWAIT;
}

void get_packet(t_ping *ping, t_time *time) {
	t_res	response;
    ssize_t	ret;
    bool    csfailed = false;

    init_header(&response, ping); 
    while (g_signals.running)
    {
        ret = recvmsg(ping->sockfd, &response.msg, MSG_DONTWAIT);
        // printf("recv2: pid=%d\n", ping->pckt.hdr->un.echo.id);
        if (ret > 0)
        {
            if (ping->pckt.hdr->type == ICMP_ECHOREPLY) {
                int sent_checksum = ping->pckt.hdr->checksum;
                ping->pckt.hdr->checksum = 0;
                int calc_checksum = checksum((unsigned short*)ping->pckt.hdr, sizeof(struct icmphdr), 0);
                csfailed = !(sent_checksum == calc_checksum);
                if (ping->pckt.hdr->un.echo.id == ping->pid)
                {
                    double rtt = calc_rtt(ping, time);
                    display_receive_msg(ret, ping, rtt, csfailed);
                    if (ping->flags & FLAG_o)
                        g_signals.running = 0;
                } else { // else if (ping->flags & FLAG_V)
                    display_receive_msg_v(ret, ping, csfailed);
                }
            }
            break;
        }
    }
}

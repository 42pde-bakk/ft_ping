//
// Created by Peer De bakker on 1/19/22.
//

#include "ft_ping.h"
#include <sys/socket.h>
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
    ssize_t	ret = 0;

    init_header(&response, ping);
    while (g_signals.running) {
		ret = recvmsg(ping->sockfd, &response.msg, response.msg.msg_flags);
        if (ret > 0) {
            if (ping->pckt.hdr->type == ICMP_ECHOREPLY) {
                ping->pckt.hdr->checksum = 0;
                ping->pckt.hdr->checksum = checksum((unsigned short*)ping->pckt.hdr, sizeof(struct icmphdr), 0);
                if (ping->pckt.hdr->un.echo.id == ping->pid)
                {
                    ping->received++;
					int sent_checksum = ping->pckt.hdr->checksum;
					bool csfailed = (sent_checksum != ping->pckt.hdr->checksum);
					double rtt = calc_rtt(time);
					time->ewma = calc_ewma(time->ewma, rtt, ping->received - 1);
					display_receive_msg(ret, ping, rtt, csfailed);
                    if (ping->flags & FLAG_o) {
                        g_signals.running = 0;
                    }
				} else {
					get_packet(ping, time);
				}
                break;
            } else if (ping->flags & FLAG_v) {
                display_msg_wrongtype(ping);
            }
        }
        if (g_signals.sigquit) {
            print_sigquit_statistics(ping, time);
        }
        if (g_signals.send) {
            printf("Request timeout for icmp_seq %u\n", ping->seq - 1);
            // Doing this to replicate pings behaviour when pinging a valid non-functional IPv4 address
            break;
        }
    }
}

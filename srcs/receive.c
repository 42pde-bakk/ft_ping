//
// Created by Peer De bakker on 1/19/22.
//

#include "ft_ping.h"
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
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

static char			*net_ntoa(struct in_addr in)
{
	static char		buffer[18];
	unsigned char	*bytes = (unsigned char *) &in;

	snprintf(buffer, sizeof(buffer), "%d.%d.%d.%d", \
		bytes[0], bytes[1], bytes[2], bytes[3]);

	return (buffer);
}

void    print_error_recv(t_ping* ping) {
    static const char	*icmp_responses[] = {
        [ICMP_DEST_UNREACH]		= "Destination Unreachable",
        [ICMP_SOURCE_QUENCH]	= "Source Quench",
        [ICMP_REDIRECT]			= "Redirect (change route)",
        [ICMP_ECHO]				= "Echo Request",
        [ICMP_TIME_EXCEEDED]	= "Time to live exceeded",
        [ICMP_PARAMETERPROB]	= "Parameter Problem",
        [ICMP_TIMESTAMP]		= "Timestamp Request",
        [ICMP_TIMESTAMPREPLY]	= "Timestamp Reply",
        [ICMP_INFO_REQUEST]		= "Information Request",
        [ICMP_INFO_REPLY]		= "Information Reply",
        [ICMP_ADDRESS]			= "Address Mask Request",
        [ICMP_ADDRESSREPLY]		= "Address Mask Reply"
    };
    const char* sender = net_ntoa((struct in_addr) {.s_addr = ping->pckt.ip->saddr});

    if (ping->pckt.hdr->type == ICMP_ECHO)
        return ;

    dprintf(STDERR_FILENO, "From %s icmp_seq=%hu %s\n",
        sender,
        ntohs(ping->pckt.hdr->un.echo.sequence),
        icmp_responses[ping->pckt.hdr->type]
    );
    ++ping->errors;
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
            ping->received++;
            if (ping->pckt.hdr->type == ICMP_ECHOREPLY) {
                int sent_checksum = ping->pckt.hdr->checksum;
                ping->pckt.hdr->checksum = 0;
                int calc_checksum = checksum((unsigned short*)ping->pckt.hdr, sizeof(struct icmphdr), 0);
                csfailed = (sent_checksum != calc_checksum);
                double rtt = calc_rtt(time);
                if (ping->pckt.hdr->un.echo.id == ping->pid)
                {
                    display_receive_msg(ret, ping, rtt, csfailed);
                    if (ping->flags & FLAG_o)
                        g_signals.running = 0;
                } else {
					get_packet(ping, time);
				}
                break;
            } else
                print_error_recv(ping);
            // break;
        }
    }
}

#include "ft_ping.h"
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

static long double llsqrt(long long a) {
    long long prev = ~((long long) 1 << 63);
    long long x = a;

    if (x > 0) {
        while (x < prev) {
            prev = x;
            x = (x + (a / x)) / 2;
        }
    }
	return (long double)(x) / 1000;
}

static long double	get_mdev(t_time *t, unsigned int received) {
	long double avg = t->sum / received;
	long double avg2 = t->sum_square / received;

	return (llsqrt(1000000 * (avg2 - avg * avg)));
}

float   get_packet_loss(t_ping* ping) {
    return (float)(ping->seq - ping->received) / (float)ping->seq * 100;
}

void    print_sigquit_statistics(t_ping* ping, t_time* time) {
    float packet_loss = get_packet_loss(ping);
    double avg = (double)(time->sum / ping->received);

    g_signals.sigquit = 0;
    if (avg != avg) // NaN check
        avg = 0;
    printf("%d/%d packets, ", ping->received, ping->seq);
    printf("%.*f%% packet loss, ", (packet_loss > 0.01f && packet_loss < 99.99f), packet_loss);
    printf("min/avg/ewma/max = %.3f/%.3f/%.3f/%.3f ms\n", time->min, avg, time->ewma, time->max);
}

void	print_statistics(t_ping* ping, t_time* time) {
	float packet_loss = get_packet_loss(ping);
    double avg = (double)(time->sum / ping->received);

	printf("--- %s ft_ping statistics ---\n", ping->host);
	printf("%d packets transmitted, ", ping->seq);
	printf("%d received, ", ping->received);
	if (ping->errors) {
	    printf("+%d errors, ", ping->errors);
	}
    printf("%.*f%% packet loss, ", (packet_loss > 0.01f && packet_loss < 99.99f), packet_loss);
	printf("time %u ms\n", timeval_difference(time->time_start, time->r));
	if (ping->received > 0) {
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3Lf ms\n",
               time->min,
               (avg == avg) ? avg : 0.0f, // NaN check
               time->max,
               get_mdev(time, ping->received)
        );
	}
}

static const char*	iphdr_format = \
	"\033[36m IP HEADER\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m| IPv%hhu | IHL %hhu |    TOS %hhu     |          Total Length    %hu   |\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m|         Identification    %04hx      |   Fragment Offset  %hx    |\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m|   TTL   %hhx    |   Protocol %hhx  |       Header Checksum %04hx    |\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m|                     Source Address    %08x                |\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m|                  Destination Address  %08x                |\n" \
	"\033[37m*****************************************************************\n";

static const char* icmphdr_format =
	"\033[36m ICMP HEADER\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m|    Type %u     |    Code %u     |        Checksum 0x%02x        |\n" \
	"\033[37m*****************************************************************\n" \
	"\033[36m|          Identifier %u      |       Sequence Number %u      |\n" \
	"\033[37m*****************************************************************\033[0m\n";

void	print_iphdr(struct iphdr* ip) {
	printf(iphdr_format,
		   ip->version, ip->ihl, ip->tos, ip->tot_len,
		   ip->id, ip->frag_off,
		   ip->ttl, ip->protocol, ip->check,
		   ip->saddr,
		   ip->daddr
   );
}

void	print_ip_icmp_packet(t_pckt* pckt) {
	struct iphdr* ip = pckt->ip;
	struct icmphdr* icmp = pckt->hdr;

	print_iphdr(ip);
	printf(icmphdr_format,
		   icmp->type, icmp->code, icmp->checksum,
		   (unsigned int)icmp->un.echo.id, (unsigned int)icmp->un.echo.sequence
   );
}

void	display_receive_msg(ssize_t ret, t_ping* ping, double rtt, bool csfailed) {
	if (ping->flags & FLAG_q)
		return;
	printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms",
		ret - sizeof(struct iphdr),
		ping->addrstr,
		ping->pckt.hdr->un.echo.sequence + 1,
		ping->pckt.ip->ttl,
		rtt
	);
	if (csfailed)
		printf(" ((CHECKSUM FAILED))");
	printf("\n");
}

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

void    display_msg_wrongtype(t_ping* ping) {
    unsigned int type = ping->pckt.hdr->type;
    printf("From %s icmp_seq=%u %s\n", ping->addrstr, ping->seq - 1, icmp_responses[type]);
}

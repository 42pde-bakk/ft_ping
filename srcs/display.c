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

void	print_statistics(t_ping* ping, t_time* time) {
	// struct timeval time_end;

	// save_current_time(&time_end);
	float packet_loss = (ping->seq - ping->received) / ping->seq * 100;

	printf("--- %s ft_ping statistics ---\n", ping->host);
	if (packet_loss < 0.01f || packet_loss > 99.99f)
		printf("%d packets transmitted, %d received, %.0f%% packet loss, ", ping->seq, ping->received, packet_loss);
	else
		printf("%d packets transmitted, %d received, %.1f%% packet loss, ", ping->seq, ping->received, packet_loss);
	printf("time %u ms\n", timeval_difference(time->time_start, time->r));
	t_time* t = time;
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3Lf ms\n",
		t->min,
		(float)(t->sum / ping->received),
		t->max,
		get_mdev(time, ping->received)
	);
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

void	print_ip_icmp_packet(t_pckt* pckt)
{
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

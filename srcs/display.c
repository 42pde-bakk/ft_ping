#include "ft_ping.h"
#include <sys/time.h>
#include <stdio.h>

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

static long double	get_mdev(t_time t, unsigned int received) {
	long double avg = t.sum / received;
	long double avg2 = t.sum_square / received;

	return (llsqrt(1000000 * (avg2 - avg * avg)));
}

void	print_statistics(t_ping* ping) {
	struct timeval time_end;

	save_current_time(&time_end);
	float packet_loss = (ping->seq - ping->received) / ping->seq * 100;

	printf("--- %s ping statistics ---\n", ping->host);
	if (packet_loss < 0.01f || packet_loss > 99.99f)
		printf("%d packets transmitted, %d received, %.0f%% packet loss, ", ping->seq, ping->received, packet_loss);
	else
		printf("%d packets transmitted, %d received, %.1f%% packet loss, ", ping->seq, ping->received, packet_loss);
	printf("time %u ms\n", timeval_difference(ping->time.time_start, time_end));
	t_time t = ping->time;
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3Lf ms\n",
		t.min,
		(float)(t.sum / ping->received),
		t.max,
		get_mdev(ping->time, ping->received)
	);
}

void	display_receive_msg_v(ssize_t ret, t_ping* ping)
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

void	display_receive_msg(ssize_t ret, t_ping* ping, double rtt) {
	printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
		ret - sizeof(struct iphdr),
		ping->addrstr,
		ping->pckt.hdr->un.echo.sequence,
		ping->pckt.ip->ttl,
		rtt
	);
	printf("ping->pckt.ip->ttl = %d\n", ping->pckt.ip->ttl);
}
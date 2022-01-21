#include "ft_ping.h"
#include <sys/time.h>
#include <stdio.h>

void	print_statistics(t_ping* ping) {
	struct timeval time_end;

	save_current_time(&time_end);
	float packet_loss = (ping->sent - ping->received) / ping->sent * 100;

	printf("--- %s ping statistics ---\n", ping->host);
	printf("%d packets transmitted, %d received, %f%% packet loss, ", ping->sent, ping->received, packet_loss);
	printf("time %u ms\n", timeval_difference(ping->time.time_start, time_end));
	t_time t = ping->time;
	printf("rtt min/avg/max/mdev = %f/%f/%f/%f ms\n",
		(float)(t.min / ping->received),
		(float)(t.avg / ping->received),
		(float)(t.max / ping->received),
		(float)(t.sum_square / ping->received)
	);
}
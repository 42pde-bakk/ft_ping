//
// Created by Peer De bakker on 1/19/22.
//

#include <sys/time.h>
#include "ft_ping.h"

void    save_current_time(struct timeval* timestamp) {
    if (gettimeofday(timestamp, NULL) == -1)
        exit_error("Error: gettimeofday");
}

unsigned int	timeval_difference(struct timeval start, struct timeval end) {
	unsigned int diff_seconds = end.tv_sec - start.tv_sec;
	int diff_useconds = end.tv_usec - start.tv_usec;

	return diff_seconds * 1000 + (int)(diff_useconds / 1000);
}

double	calc_rtt(t_ping* ping)
{
    double rtt;

	save_current_time(&ping->time.r);
    ping->received++;

    rtt = (ping->time.r.tv_usec - ping->time.s.tv_usec) / 1000000.0;
    rtt += (ping->time.r.tv_sec - ping->time.s.tv_sec);
    rtt *= 1000.0;
    if (rtt > ping->time.max)
        ping->time.max = rtt;
    if (ping->time.min == 0 || rtt < ping->time.min)
        ping->time.min = rtt;
    ping->time.sum += rtt;
    ping->time.sum_square += rtt * rtt;
	return rtt;
}

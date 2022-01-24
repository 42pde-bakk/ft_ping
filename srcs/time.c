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

double calc_rtt(t_time* time)
{
    double rtt;

	save_current_time(&time->r);

    rtt = (double)(time->r.tv_usec - time->s.tv_usec) / 1000000.0;
    rtt += (double)(time->r.tv_sec - time->s.tv_sec);
    rtt *= 1000.0;
    if (rtt > time->max)
        time->max = rtt;
    if (time->min == 0 || rtt < time->min)
        time->min = rtt;
    time->sum += rtt;
    time->sum_square += rtt * rtt;
	return rtt;
}

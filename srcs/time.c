//
// Created by Peer De bakker on 1/19/22.
//

#include <sys/time.h>
#include "ft_ping.h"

void    save_current_time(struct timeval* timestamp) {
    if (gettimeofday(timestamp, NULL) == -1)
        exit_error("Error: gettimeofday");
}

void	calc_rtt(t_ping* ping)
{
    long double rtt = 0.0;

	save_current_time(&ping->time.r);

    ping->received++;
    rtt = (ping->time.r.tv_usec - ping->time.s.tv_usec) / 1000000.0;
    rtt += (ping->time.r.tv_sec - ping->time.s.tv_sec);
    rtt *= 1000.0;
    ping->time.rtt = rtt;
    if (rtt > ping->time.max)
        ping->time.max = rtt;
    if (rtt < ping->time.min || ping->time.min == 0.0)
        ping->time.min = rtt;
    ping->time.avg += rtt;
    ping->time.sum_square += rtt * rtt;
}

//
// Created by Peer De bakker on 1/19/22.
//

#include <sys/time.h>
#include "ft_ping.h"

void    save_current_time(struct timeval* timestamp) {
    if (gettimeofday(timestamp, NULL) == -1)
        exit_error("Error: gettimeofday");
}

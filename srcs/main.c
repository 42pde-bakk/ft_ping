#include <stdio.h>
#include <unistd.h>
#include "ft_ping.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
t_signals g_signals;

t_ping* init_ping(void) {
	t_ping* ping = calloc(1, sizeof(t_ping));
	bzero(ping, sizeof(t_ping));
	if (!ping)
		exit_error("Error mallocing for ping struct");
    ping->pckt.ip = (struct iphdr *)ping->pckt.buf;
    ping->pckt.hdr = (struct icmphdr *)(ping->pckt.ip + 1);
    ping->pid = getpid();
    printf("ping->pid = %d\n", ping->pid);
    ping->seq = 0;
    ping->ttl = TTL;
    ping->count = -1;
    ping->interval = 1;

	g_signals.running = 1;
	g_signals.send = 1;
	return (ping);
}


void    start_ping(t_ping* ping) {
//    struct timeval  timestamp_start;
//    struct timeval  timestamp_end;

    ping->sockfd = create_socket();
    printf("PING %s (%s) %d(%d) bytes of data.\n", ping->host, ping->addrstr, 56, PACKET_SIZE);

	// save_current_time(&ping->time.time_start);

    while (g_signals.running) {
        if (g_signals.send) {
            send_packet(ping);
            alarm(ping->interval);
            get_packet(ping);
        }
    }
	print_statistics(ping);
}

int main(int argc, char** argv) {
	t_ping*	ping = init_ping();

	if (getuid() != 0) {
		exit_error("Error. Must run program as root to be able to open raw sockets.");
	}

	if (argc < 2) {
		exit_error(get_usage_string());
	}
    if (parse_argv(argc, argv, ping)) {
        exit(EXIT_FAILURE);
	}
    set_signal_handlers();
    start_ping(ping);
    exit(EXIT_SUCCESS);
}

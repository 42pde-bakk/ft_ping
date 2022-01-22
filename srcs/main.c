#include <stdio.h>
#include <unistd.h>
#include "ft_ping.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
t_signals g_signals;

t_ping* init_ping(void) {
	t_ping* ping = ft_calloc(1, sizeof(t_ping));
	ft_bzero(ping, sizeof(t_ping));
	if (!ping)
		exit_error("Error mallocing for ping struct");
    ping->pckt.ip = (struct iphdr *)ping->pckt.buf;
    ping->pckt.hdr = (struct icmphdr *)(ping->pckt.ip + 1);
    ping->pid = getpid();
	if (getpid() > UINT16_MAX)
		exit_error("Error, wont be able to save pid value in iphdr");
    ping->seq = 0;
    ping->ttl = TTL;
    ping->interval = 1;

	g_signals.running = 1;
	g_signals.send = 1;
	return (ping);
}

void	cleanup_ping(t_ping* ping) {
	close_socket(ping);
	exit(EXIT_SUCCESS);
}


void    start_ping(t_ping* ping) {
	t_time time;
	ft_bzero(&time, sizeof(t_time));

    printf("PING %s (%s) %d(%d) bytes of data.\n", ping->host, ping->addrstr, 56, PACKET_SIZE);
    while (g_signals.running) {
        if (g_signals.send) {
            send_packet(ping, &time);
			alarm(ping->interval);
            get_packet(ping, &time);
        }
    }
	print_statistics(ping, &time);
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
	ping->sockfd = create_socket();
    set_signal_handlers();
    start_ping(ping);

	cleanup_ping(ping);
}

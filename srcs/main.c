#include <stdio.h>
#include <unistd.h>
#include "ft_ping.h"
#include <stdlib.h>
#include <string.h>
t_signals g_signals = {
        .send = 1,
        .finito = 0
};

t_ping* init_ping(void) {
	t_ping* ping = ft_calloc(1, sizeof(t_ping));
	if (!ping)
		exit_error("Error mallocing for ping struct");
//	ping->pckt.ip = (struct iphdr*)ping->pckt.buf;
    ping->pckt.ip = (struct iphdr *)ping->pckt.buf;
    ping->pckt.hdr = (struct icmphdr *)(ping->pckt.ip + 1);
    ping->pid = getpid();
    ping->seq = 1;
    ping->time.min = 0.0;
    ping->time.max = 0.0;
    ping->time.sum_square = 0;
    ping->ttl = TTL;
    ping->count = -1;
    ping->interval = 1;
	return (ping);
}


void    start_ping(t_ping* ping) {
    ping->sockfd = create_socket();
    printf("PING %s (%s) %d(%d) bytes of data.\n", ping->host, ping->addrstr, NAKED_PACKET_SIZE, PACKET_SIZE);

    while (!g_signals.finito) {
        if (g_signals.send) {
            send_packet(ping);
            alarm(1);
            get_packet(ping);
        }
    }
}

int main(int argc, char** argv) {
	t_ping*	ping = init_ping();

	if (argc < 2) {
		exit_error(get_usage_string());
	}
    if (parse_argv(argc, argv, ping))
        exit(EXIT_FAILURE);
    set_signal_handlers();
    start_ping(ping);
    exit(EXIT_SUCCESS);
}

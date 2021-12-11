//
// Created by Peer De bakker on 11/17/21.
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "ft_ping.h"
#include "print_addr.h"

t_signals g_signals;

void    ping_loop(t_ping* ping) {
    send_packet(ping);
    receive_message(ping);
}

void    init_ping(t_ping* ping) {
    memset(ping, 0, sizeof(t_ping));
}

int main(int argc, char** argv) {
    t_ping  ping;
    struct sockaddr_in  addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    memset(&ping, 0, sizeof(t_ping));

    if (argc < 2) {
        parse_error();
    }
    // parsing
    if (parse_input(&ping, argv)) {
        return (EXIT_FAILURE);
    }
    ping.socketFd = create_socket();
    set_signal_handlers();


    int data_padding_bytes = 56;
    // print initial thingy
//    get_sockaddr(ping.addrInfo->ai_addr);
    printf("PING %s (%s): %d data bytes\n", argv[1], ping.destination_address, data_padding_bytes);

    ping_loop(&ping);


	return (0);
}

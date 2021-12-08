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

int main(int argc, char** argv) {
    t_ping  ping;
    struct in_addr ip;
    struct sockaddr_in  addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    memset(&ping, 0, sizeof(t_ping));

    if (argc < 2) {
        parse_error();
    }
    ping.pid = getpid();
    // parsing

    set_signal_handlers();

    resolve_hostname(argv[1], &ip);
    inet_ntop(AF_INET, &ping.addrInfo->ai_addr, ping.addrstr, INET_ADDRSTRLEN);
    printf("after! addrstr='%s'\n", ping.addrstr);
    print_addrinfo(1, ping.addrInfo, "");
    ping.socketFd = create_socket(ping.addrInfo);

    int data_padding_bytes = 56;
    // print initial thingy
//    get_sockaddr(ping.addrInfo->ai_addr);
    printf("PING %s (%s): %d data bytes\n", argv[1], ping.address, data_padding_bytes);

    ping_loop(&ping);


	return (0);
}

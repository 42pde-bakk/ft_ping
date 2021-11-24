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

    while (!g_signals.sigint) {
        send_packet(ping);
    }
}

int main(int argc, char** argv) {
    t_ping  ping;
    char*   hostname;
    memset(&ping, 0, sizeof(t_ping));

    if (argc < 2) {
        parse_error();
    }
    hostname = argv[1];
    ping.pid = getpid();
    // parsing

    set_signal_handlers();

    ping.addrInfo = get_addrinfo(hostname, &ping);
    inet_ntop(AF_INET, &ping.addrInfo->ai_addr, ping.addrstr, INET_ADDRSTRLEN);
    printf("after! addrstr='%s'\n", ping.addrstr);
    print_addrinfo(1, ping.addrInfo, "");
    ping.socketFd = create_socket(ping.addrInfo);

    int data_padding_bytes = 56;
    // print initial thingy
    ping.sockaddr = get_sockaddr(ping.addrInfo->ai_addr);
    printf("PING %s (%s): %d data bytes\n", hostname, ping.sockaddr, data_padding_bytes);

//    send_packet(socketFd, ping.addrInfo);
    send_packet(&ping);

    // while true
        // send packet
        // recv


	return (0);
}

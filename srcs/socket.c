//
// Created by Peer De bakker on 11/19/21.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include "ft_ping.h"
#include <stdio.h>
#include <unistd.h>

void close_socket(int socketFd) {
    if (close(socketFd)) {
        perror("close");
    }
}

int	create_socket() {
    const int socketFd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (socketFd < 0) {
        perror("socket");
        return (-1);
    }
    if (setsockopt(socketFd, IPPROTO_IP, IP_HDRINCL, (int[1]){1}, sizeof(int)) == -1) {
        perror("setsockopt");
        close_socket(socketFd);
        return (-1);
    }
    return (socketFd);
}

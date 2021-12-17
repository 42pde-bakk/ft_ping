//
// Created by Peer De bakker on 12/17/21.
//
#include "ft_ping.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void    close_socket(int fd) {
    close(fd);
}

int create_socket(void) {
    int socketFd;
    int opt_val = 1;

    socketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socketFd == -1) {
        exit_error("Error setting socket file descriptor");
    }
    if (setsockopt(socketFd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int)) < 0) {
        exit_error("Error setting IP_HDRINCL in socket options");
    }
    return socketFd;
}

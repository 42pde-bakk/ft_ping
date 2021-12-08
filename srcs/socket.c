//
// Created by Peer De bakker on 11/19/21.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "ft_ping.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int resolve_hostname(const char *hostname, struct in_addr *ip) {
    struct addrinfo hints,
                    *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
//    hints.ai_protocol = IPPROTO_ICMP;
//    hints.ai_flags = AI_CANONNAME;

    if (getaddrinfo(hostname, NULL, &hints, &result))
        exit_error("Error. getaddrinfo failed");
    *ip = ((struct sockaddr_in *)result->ai_addr)->sin_addr;
    free(result);
    return (0);
}

void close_socket(int socketFd) {
    close(socketFd);
}

int	create_socket(struct addrinfo* result) {
    (void)result;
//	const int socketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	const int socketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int opt_val = 1;

	if (socketFd == -1) {
		dprintf(2, " error: %s\n", strerror(errno)); // TODO: remove
		exit_error("Error creating socket File Descriptor");
	}
	if (setsockopt(socketFd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int)) < 0) {
		exit_error("setsockopt");
	}
	// save in struct?
	return (socketFd);
}


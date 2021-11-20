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

int	create_socket(struct addrinfo* result) {
    (void)result;
//	const int socketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	const int socketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int opt_val = 1;

	if (socketFd == -1) {
		dprintf(2, " error: %s\n", strerror(errno));
		exit_error("creating socket File Descriptor");
	}
	// setsockopt
	if (setsockopt(socketFd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int)) < 0) {
		exit_error("setsockopt");
	}
	// save in struct?
	return (socketFd);
}

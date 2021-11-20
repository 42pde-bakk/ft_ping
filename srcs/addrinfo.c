//
// Created by Peer De bakker on 11/19/21.
//
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "ft_ping.h"
//int				get_addrinfo(char *av)
//{
//	struct addrinfo hints;
//	struct addrinfo *result;
//
//	ft_bzero(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_RAW;
//	hints.ai_protocol = IPPROTO_ICMP;
//	if (getaddrinfo(av, NULL, &hints, &result) != 0)
//		return (1);
//	g_params->rec_in = (struct sockaddr_in *)result->ai_addr;
//	return (0);
//}

struct addrinfo *get_addrinfo(const char *hostname) {
	struct addrinfo hints, *result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags = 0;

	int error = getaddrinfo(hostname, NULL, &hints, &result);
	if (error != 0) {
		dprintf(STDERR_FILENO, "getaddrinfo: %s\n", gai_strerror(error));
		exit(EXIT_FAILURE);
	}
	return (result);
}

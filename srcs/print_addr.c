//
// Created by Peer De bakker on 11/17/21.
//

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

void    print_sockaddr(int fd, struct sockaddr* res, const char* prefix) {
	char s[INET6_ADDRSTRLEN > INET_ADDRSTRLEN ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN] = "\0";
	switch(res->sa_family) {
		case AF_INET: {
			struct sockaddr_in *addr_in = (struct sockaddr_in *)res;

			////char s[INET_ADDRSTRLEN] = '\0';
			// this is large enough to include terminating null

			inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
			break;
		}
		case AF_INET6: {
			struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res;

			////char s[INET6_ADDRSTRLEN] = '\0';
			// not sure if large enough to include terminating null?

			inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
			break;
		}
		default:
			break;
	}
	dprintf(fd, "%ssockaddr: %s\n", prefix, s);
}

void    print_addrinfo(int fd, struct addrinfo* ai, const char* prefix) {
	dprintf(fd, "%sai_flags: %d\n", prefix, ai->ai_flags);
	dprintf(fd, "%sai_family: %d\n", prefix, ai->ai_family);
	dprintf(fd, "%sai_socktype: %d\n", prefix, ai->ai_socktype);
	dprintf(fd, "%sai_protocol: %d\n", prefix, ai->ai_protocol);
	dprintf(fd, "%sai_addrlen: %d\n", prefix, ai->ai_addrlen);
	dprintf(fd, "%sai_protocol: %d\n", prefix, ai->ai_protocol);
	dprintf(fd, "%sai_protocol: %d\n", prefix, ai->ai_protocol);
	print_sockaddr(fd, ai->ai_addr, prefix);
	dprintf(fd, "%sai_canonname: %s\n", prefix, ai->ai_canonname);
}
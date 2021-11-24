//
// Created by Peer De bakker on 11/19/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>

void	exit_error(const char* s) {
	dprintf(STDERR_FILENO, "%s\n", s);
	exit(EXIT_FAILURE);
}

void    fill_timestamp(void* buffer) {
    if (gettimeofday(buffer, NULL) == -1) {
        exit_error("Error filling timestamp");
    }
}

const char* get_sockaddr(struct sockaddr* res) {
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
    char* ret = calloc(sizeof(s) + 1, sizeof(char));
    return strcpy(ret, s); //TODO; make sure not to leak
}

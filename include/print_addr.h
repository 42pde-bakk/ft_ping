//
// Created by Peer De bakker on 11/17/21.
//

#ifndef FT_PING_PRINT_ADDR_H
#define FT_PING_PRINT_ADDR_H

#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

void    print_sockaddr(int fd, struct sockaddr* res, const char* prefix);
void    print_addrinfo(int fd, struct addrinfo* ai, const char* prefix);

#endif //FT_PING_PRINT_ADDR_H

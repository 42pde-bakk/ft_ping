//
// Created by Peer De bakker on 11/17/21.
//

#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "print_addr.h"

static struct addrinfo*  get_addrinfo(const char* hostname) {
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int er = getaddrinfo(hostname, "http", &hints, &result);
    if (er != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(er));
        exit(EXIT_FAILURE);
    }
    return (result);
}

static void connecttosocket(struct addrinfo* result) {
    int sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    char payload[16] = {0};

    if (sendto(sock, payload, 16, 0, result->ai_addr, result->ai_addrlen) == -1)
        perror("sendto");
}


int main(int argc, char** argv) {
    struct addrinfo* addrInfo = get_addrinfo("tracker.openbittorrent.com");
    connecttosocket(addrInfo);


    return (0);
}

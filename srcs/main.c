//
// Created by Peer De bakker on 11/17/21.
//

#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "ft_ping.h"
#include "print_addr.h"

t_signals g_signals;

static int wide_receiver(int socketfd, struct addrinfo* addrInfo) {
	struct msghdr   msg;
	struct iovec    iov;
    ssize_t         size;
    char            recvbuf[1500] = {0};
	char            controlbuf[BUFSIZ] = {0};
	int             s;
    struct timeval  tv_start,
                    tv_end;

	memset(&msg, 0, sizeof(msg));
	memset(&iov, 0, sizeof(iov));

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);

    msg.msg_name = addrInfo->ai_addr;
    msg.msg_namelen = addrInfo->ai_addrlen;
    iov.iov_base = recvbuf;
    //replace sizeof(data) by MAX
    iov.iov_len = iov.iov_len;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = controlbuf;
    msg.msg_controllen = sizeof(controlbuf);
    msg.msg_flags = 0;

    s = recvmsg(socketfd, &msg, 0);
    if (s < 0) {
        perror("recvmsg");
        s = 0;
    } else {
        printf("no perror\n");
    }
    return s;
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    char addrstr[INET6_ADDRSTRLEN];
    t_ping ping;
    memset(&ping, 0, sizeof(t_ping));
    char* hostname = "google.com";
//	int sock = connecttosocket(addrInfo);
//    int s = wide_receiver(sock, addrInfo);

    // parsing

    // set up signal handler

    struct addrinfo* addrInfo = get_addrinfo("8.8.8.8");
    print_addrinfo(1, addrInfo, "");
//    inet_ntop(AF_INET, (void*)&ping.addr->sin_addr, addrstr, INET6_ADDRSTRLEN);
    printf("after\n");
    print_addrinfo(1, addrInfo, "");
    int socketFd = create_socket(addrInfo);

//    print_addrinfo(1, addrInfo, "");
    // print initial thingy
    printf("PING %s ..: %d data bytes\n", hostname, 56);

    // while true
        // send packet
        // recv


	return (0);
}

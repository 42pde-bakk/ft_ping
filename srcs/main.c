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

int wide_receiver(int socketfd, struct addrinfo* addrInfo) {
	struct msghdr   msg;
	struct iovec    iov;
    ssize_t         size;
    char            recvbuf[1500] = {0};
	char            controlbuf[BUFSIZ] = {0};
	int             s;
    struct timeval  tv_start,
                    tv_end;

    (void)size;
    (void)tv_start;
    (void)tv_end;

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
    char addrstr[INET_ADDRSTRLEN];
    (void)addrstr;
    t_ping ping;
    if (argc < 2) {
       parse_error();
    }
    memset(&ping, 0, sizeof(t_ping));
    char* hostname = argv[1];
//	int sock = connecttosocket(addrInfo);
//    int s = wide_receiver(sock, addrInfo);

    // parsing

    // set up signal handler
    set_signal_handlers();

    struct addrinfo* addrInfo = get_addrinfo(hostname, &ping);
    print_addrinfo(1, addrInfo, "");
    inet_ntop(AF_INET, &addrInfo->ai_addr, addrstr, INET_ADDRSTRLEN);
    printf("after! addrstr='%s'\n", addrstr);
    print_addrinfo(1, addrInfo, "");
    int socketFd = create_socket(addrInfo);
    (void)socketFd;

//    print_addrinfo(1, addrInfo, "");
    // print initial thingy
    printf("PING %s ..: %d data bytes\n", hostname, 56);
    send_packet(socketFd, addrInfo);

    // while true
        // send packet
        // recv


	return (0);
}

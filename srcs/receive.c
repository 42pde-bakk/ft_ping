//
// Created by Peer De bakker on 11/26/21.
//
#include "ft_ping.h"
#include <string.h>
#include <stdio.h>

void    initialize_response(t_echoreply* reply, t_ping* ping) {
    (void)ping;
    memset(reply, 0, sizeof(t_echoreply));

    reply->iov.iov_base = reply->buf;
    reply->iov.iov_len = sizeof(reply->buf);
//    reply->msghdr.msg_name = ping->addrstr;
//    reply->msghdr.msg_name = ping->addrInfo->ai_addr;
//    reply->msghdr.msg_namelen = sizeof(ping->packet);
//    reply->msghdr.msg_namelen = ping->addrInfo->ai_addrlen;
    reply->msghdr.msg_iov = &reply->iov;
    reply->msghdr.msg_iovlen = 1;
    reply->msghdr.msg_flags = 0;
    reply->msghdr.msg_control = reply->buf;
    reply->msghdr.msg_controllen = sizeof(reply->buf);
}

int receive_message(t_ping* ping) {
    char    buffer[512];
    ssize_t ret;
    char    rcvd_packet[PACKET_SIZE];
    struct iovec io = {
            .iov_base = rcvd_packet,
            .iov_len = PACKET_SIZE
    };
    struct msghdr msg = {
            .msg_name = &ping->dest,
            .msg_namelen = sizeof(ping->dest),
            .msg_iov = &io,
            .msg_iovlen = 1,
            .msg_control = buffer,
            .msg_controllen = sizeof(buffer),
            .msg_flags = 0
    };
    ret = recvmsg(ping->socketFd, &msg, 0);
    if (ret == -1) {
        perror("recvmsg");
    }
}

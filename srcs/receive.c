//
// Created by Peer De bakker on 11/26/21.
//
#include "ft_ping.h"
#include <string.h>
#include <stdio.h>

void    initialize_response(t_echoreply* reply, t_ping* ping) {
    memset(reply, 0, sizeof(t_echoreply));

    reply->iov.iov_base = reply->buf;
    reply->iov.iov_len = sizeof(reply->buf);
//    reply->msghdr.msg_name = ping->addrstr;
    reply->msghdr.msg_name = ping->addrInfo->ai_addr;
//    reply->msghdr.msg_namelen = sizeof(ping->packet);
    reply->msghdr.msg_namelen = ping->addrInfo->ai_addrlen;
    reply->msghdr.msg_iov = &reply->iov;
    reply->msghdr.msg_iovlen = 1;
    reply->msghdr.msg_flags = 0;
    reply->msghdr.msg_control = reply->buf;
    reply->msghdr.msg_controllen = sizeof(reply->buf);
}

int receive_message(t_ping* ping) {
    t_echoreply reply;

    initialize_response(&reply, ping);
    ssize_t bytes_received = recvmsg(ping->socketFd, &(reply.msghdr), MSG_ERRQUEUE);
    if (bytes_received > 0) {
        // success
    } else {
        // Error
        perror("recvmsg");
    }
    return (0);
}

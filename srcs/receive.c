//
// Created by Peer De bakker on 11/26/21.
//
#include "ft_ping.h"
#include <string.h>
#include <stdio.h>

int receive_message(t_ping* ping) {
	t_response	res;
	
	memset(&res, 0, sizeof(t_response));
	memset((void*)ping->packet.buf, 0, PACKET_SIZE);

	res.iov->iov_base = (void*)ping->packet.buf;
	res.iov->iov_len = sizeof(ping->packet.buf);

	res.msg.msg_iov = res.iov;
	res.msg.msg_iovlen = 1;
	res.msg.msg_name = NULL;
	res.msg.msg_namelen = 0;
	res.msg.msg_flags = 0;


	ssize_t ret = recvmsg(ping->socketFd, &res.msg, 0);
	if (ret < 0) {
		perror("recvmsg");
	}
}

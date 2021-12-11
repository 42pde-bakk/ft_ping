//
// Created by Peer De bakker on 11/19/21.
//

#ifndef FT_PING_FT_PING_H
# define FT_PING_FT_PING_H
# include <stdbool.h>
# include <netdb.h>
//# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/icmp6.h>

# define IP_HDR_SIZE			20
# define ICMP_HDR_SIZE			ICMP_MINLEN
# define ICMP_PAYLOAD_SIZE		56
# define PACKET_SIZE			(IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_PAYLOAD_SIZE)
# define ALIGN_TIMESTAMP		4


typedef struct  s_signals {
	bool	sigint,
			sigalarm;
}               t_signals;

typedef struct  s_packet {
    char        buf[16];
    struct icmp icmp_header;
}               t_packet;

typedef struct  s_echoreply {
    struct msghdr   msghdr;
    struct iovec    iov;
    struct icmp*    icmp;
    char            buf[2000];
    int             received_bytes;
}               t_echoreply;

extern t_signals g_signals;
void    set_signal_handlers(void);

typedef struct s_ping {
	struct sockaddr_in	dest;
	const char*         destination_address;
	int                 socketFd;
	size_t              sequence;
} t_ping;

int     create_socket();
void    close_socket(int socketFd);

int    send_packet(t_ping* ping);
int receive_message(t_ping* ping);

/*
 * ** Util functions:
 */
void    parse_error(void);
int     parse_input(t_ping* ping, char** argv);
void	exit_error(const char* s);
void	gen_ip_header(void *packet, u_int32_t dest);
void	gen_icmp_msg(void *packet, int seq);

#endif //FT_PING_FT_PING_H

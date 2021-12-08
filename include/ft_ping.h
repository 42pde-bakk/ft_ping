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
	struct sockaddr_in	sockaddr;
	char                addrstr[INET_ADDRSTRLEN];
	pid_t               pid;
	size_t              messageCount;
    struct addrinfo*    addrInfo;
    char*               address;
    t_packet            packet;

    char*               hostname;
    int                 socketFd;
    size_t              package_count,
                        packages_transmitted,
                        packages_received;
    struct timeval      time_start;
    struct sockaddr_in  addr;

} t_ping;

int	create_socket(struct addrinfo* addrInfo);
int resolve_hostname(const char *hostname, struct in_addr* ip);

int    send_packet(t_ping* ping);
int receive_message(t_ping* ping);

/*
 * ** Util functions:
 */
void    parse_error(void);
void	exit_error(const char* s);

#endif //FT_PING_FT_PING_H

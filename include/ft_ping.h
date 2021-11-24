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

extern t_signals g_signals;
void    set_signal_handlers(void);

typedef struct s_ping {
	struct sockaddr_in*	addr;
	char                addrstr[INET_ADDRSTRLEN];
	pid_t               pid;
	int                 socketFd;
	size_t              messageCount;
    struct addrinfo*    addrInfo;
    const char*         sockaddr;
} t_ping;

int	create_socket(struct addrinfo* addrInfo);
struct addrinfo *get_addrinfo(const char *hostname, t_ping *ping);

int    send_packet(t_ping* ping);

/*
 * ** Util functions:
 */
void    parse_error(void);
void	exit_error(const char* s);

#endif //FT_PING_FT_PING_H

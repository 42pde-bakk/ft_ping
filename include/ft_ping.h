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
# define PACKET_PING_SIZE 84
# define USEC_TIMEOUT 50000
# define TTL 17
# define FLAG_V 0x02


typedef struct  s_signals {
	bool	sigint,
			sigalarm;
}               t_signals;

typedef struct	s_pckt
{
    char			buf[PACKET_PING_SIZE];
    struct iphdr	*ip;
    struct icmphdr	*hdr;
}				t_pckt;

typedef struct	s_res
{
    struct iovec	iov[1];
    struct msghdr	msg;
}				t_res;

typedef struct	s_time
{
    struct timeval	time_start;
    struct timeval	time_end;
    struct timeval	s;
    struct timeval	r;
    long double		rtt;
    long double		min;
    long double		max;
    long double		avg;
    long double		sum_square;
}				t_time;

extern t_signals g_signals;
void    set_signal_handlers(void);

typedef struct s_ping {
	struct sockaddr_in*	rec_in;
	const char*         hostname;
	char                addrstr[INET6_ADDRSTRLEN];
    int					sockfd;
    pid_t				pid;
    int					seq;
    int					sended;
    int					reiceved;
    int					bytes;
    t_res				response;
    t_time				time;
    int					ttl;
    int					count;
    int					interval;
    int					daddr;
    unsigned char		flags;
    t_signals			signals;
} t_ping;


int	parse_argv(int argc, char** argv, t_ping* ping);


/*
 * ** Util functions:
 */
unsigned short checksum(void *b, int len);
void*       ft_calloc(size_t count, size_t size);
void	    exit_error(const char* s);
const char* get_usage_string(void);

#endif //FT_PING_FT_PING_H

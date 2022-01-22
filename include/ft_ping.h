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
#include <sys/time.h>
# define PACKET_SIZE 84
# define USEC_TIMEOUT 50000
# define TTL 64
# define FLAG_o 0x01
# define FLAG_q 0x02
# define FLAG_v 0x03



typedef struct  s_signals {

	bool	send,
			running;
}               t_signals;

typedef struct	s_pckt
{
    char			buf[PACKET_SIZE];
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
    struct timeval	s;
    struct timeval	r;
    double		min;
    double		max;
    long double	sum;
    long double	sum_square;
}				t_time;

typedef struct s_ping {
    t_pckt				pckt;
    struct sockaddr_in	*rec_in;
    char				*host;
    char				addrstr[INET6_ADDRSTRLEN];
    pid_t				pid;
    int					seq;
    int					sockfd;
    // int					sent;
    int					received;
    int					ttl;
    int					count;
    int					interval;
    int					daddr;
    unsigned char		flags;
} t_ping;

// the one true global
extern t_signals g_signals;

// time.c
void    save_current_time(struct timeval* timestamp);
double	calc_rtt(t_ping* ping, t_time* time);
unsigned int	timeval_difference(struct timeval start, struct timeval end);

// signals.c
void    set_signal_handlers(void);
void    sigalarm_handler(int dummy);
void    sigint_handler(int dummy);

// parsing.c
int	parse_argv(int argc, char** argv, t_ping* ping);

// socket.c
int create_socket(void);
void close_socket(t_ping *ping);

// send.c
void	send_packet(t_ping* ping, t_time* time);

// receive.c
void	get_packet(t_ping* ping, t_time* time);

// display.c
void	print_statistics(t_ping* ping, t_time* time);
void	display_receive_msg(ssize_t ret, t_ping* ping, double rtt);
void	display_receive_msg_v(ssize_t ret, t_ping* ping);

// utils.c
unsigned short checksum(void *b, int len);
void*       ft_calloc(size_t count, size_t size);
void		ft_bzero(void *s, size_t n);
void	    exit_error(const char* s);
void		exit_success(const char* str);
const char* get_usage_string(void);

#endif //FT_PING_FT_PING_H

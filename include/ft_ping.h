//
// Created by Peer De bakker on 11/19/21.
//

#ifndef FT_PING_FT_PING_H
# define FT_PING_FT_PING_H
# include <stdbool.h>
# include <netdb.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/icmp6.h>
# include <sys/time.h>
# include <stdio.h>
# define PACKET_SIZE 84
# define TTL 64
# define FLAG_o 1u
# define FLAG_q 2u
# define FLAG_v 4u

typedef struct  s_signals {
	bool	send,
			running,
			sigquit;
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
    double      ewma;
}				t_time;

typedef struct s_ping {
    t_pckt				pckt;
    struct sockaddr_in	*rec_in;
    char				*host;
    char				addrstr[INET6_ADDRSTRLEN];
    uint16_t			pid;
    int					seq;
    int					sockfd;
    int					received;
    int                 errors;
    int					interval;
    unsigned int		daddr;
    unsigned int   		flags;
} t_ping;

// the one true global
extern t_signals g_signals;

// time.c
void    save_current_time(struct timeval* timestamp);
double	calc_rtt(t_time* time);
double    calc_ewma(double old_ewma, double rtt, unsigned int nb);
unsigned int	timeval_difference(struct timeval start, struct timeval end);

// signals.c
void    set_signal_handlers(void);

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
void	display_receive_msg(ssize_t ret, t_ping* ping, double rtt, bool csfailed);
void    print_sigquit_statistics(t_ping* ping, t_time* time);
void	print_ip_icmp_packet(t_pckt* pckt);
void    display_msg_wrongtype(t_ping* ping);

// utils.c
unsigned short checksum(void *b, int len, unsigned short csum);
void*       ft_calloc(size_t count, size_t size);
void		ft_bzero(void *s, size_t n);
void	    exit_error(const char* s);
void		exit_success(const char* str);
const char* get_usage_string(void);

#endif //FT_PING_FT_PING_H

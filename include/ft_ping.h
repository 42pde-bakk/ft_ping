//
// Created by Peer De bakker on 11/19/21.
//

#ifndef FT_PING_FT_PING_H
# define FT_PING_FT_PING_H
# include <stdbool.h>

#if __APPLE__

//	struct iphdr {
//	#if defined(__LITTLE_ENDIAN_BITFIELD)
//		__u8    ihl:4,
//					version:4;
//	#elif defined (__BIG_ENDIAN_BITFIELD)
//		__u8    version:4,
//					ihl:4;
//	#else
//	#endif
//		__u8   tos;
//		__u16  tot_len;
//		__u16  id;
//		__u16  frag_off;
//		__u8   ttl;
//		__u8   protocol;
//		__u16  check;
//		__u32  saddr;
//		__u32  daddr;
//		/*The options start here. */
//	};
#endif

typedef struct s_signals {
	bool	sigint,
			sigalarm;

} t_signals;
extern t_signals g_signals;

typedef struct s_ping {
	struct sockaddr_in*	addr;

} t_ping;

int	create_socket(struct addrinfo* addrInfo);
struct addrinfo *get_addrinfo(const char *hostname);
/*
 * ** Util functions:
 */
void	exit_error(const char* s);

#endif //FT_PING_FT_PING_H

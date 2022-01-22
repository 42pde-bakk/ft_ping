#include "ft_ping.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>


int	obtain_addrinfo(t_ping* ping, const char* hostname) {
	struct addrinfo hints;
	struct addrinfo* result;

	ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    if (getaddrinfo(hostname, NULL, &hints, &result) != 0)
        return (1);
    ping->rec_in = (struct sockaddr_in*)result->ai_addr;
    return (0);
}

int	parse_flag(t_ping* ping, char flag) {
	switch (flag) {
		case 'h':
			exit_success(get_usage_string());
			__attribute__((fallthrough));
		case 'o':
			ping->flags |= FLAG_o;
			return (1);
		case 'q':
			ping->flags |= FLAG_q;
			return (1);
		case 'v':
			ping->flags |= FLAG_v;
			return (1);
		case 'V':
			exit_success("ft_ping utility, peerutils-s20220119");
			__attribute__((fallthrough));
		default:
			return (0);
	}
}

int	parse_argv(int argc, char** argv, t_ping* ping) {
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			for (int n = 1; argv[i][n]; ++n) {
				if (parse_flag(ping, argv[i][n]) == 0)
					exit_error(get_usage_string());
			}
		}
		else {
			ping->host = argv[i];
		    if (obtain_addrinfo(ping, ping->host)) {
		        dprintf(STDERR_FILENO, "ping: %s: Name or service not known\n", ping->host);
		        return (1);
		    }
		    inet_ntop(AF_INET, (void*)&ping->rec_in->sin_addr, ping->addrstr, INET6_ADDRSTRLEN);
		}
	}
	if (!ping->host)
		exit_error(get_usage_string());
	return (0);
}

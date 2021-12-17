#include "ft_ping.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int	obtain_addrinfo(t_ping* ping, const char* hostname) {
	struct addrinfo hints;
	struct addrinfo* result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    if (getaddrinfo(hostname, NULL, &hints, &result) != 0)
        return (1);
    ping->rec_in = (struct sockaddr_in*)result->ai_addr;
    return (0);
}

int	parse_argv(int argc, char** argv, t_ping* ping) {
	for (int i = 0; i < argc; ++i) {
		if (argv[i][0] == '-') {
			for (int n = 1; argv[i][n]; ++n) {
				if (argv[i][n] == 'h') {
					exit_error(get_usage_string());
				} else if (argv[i][n] == 'v') {
				    // handle the v flag
				} else {
				    dprintf(STDERR_FILENO, "ping: invalid option -- %c\n%s\n", argv[i][n], get_usage_string());
				    return (1);
				}
			}
		}
		else {
		    // Parse hostname
		    ping->hostname = argv[i];
		    if (obtain_addrinfo(ping, argv[i])) {
		        dprintf(STDERR_FILENO, "ping: cannot resolve %s: Unknown host\n", argv[i]);
		        return (1);
		    }
		    inet_ntop(AF_INET, (void*)&ping->rec_in->sin_addr, ping->addrstr, INET6_ADDRSTRLEN);
		}
	}
	return (0);
}

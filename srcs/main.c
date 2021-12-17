#include <stdio.h>
#include <unistd.h>
#include "ft_ping.h"
#include <stdlib.h>

int	init_ping(t_ping* ping) {
	ping = ft_calloc(1, sizeof(t_ping));
	if (!ping) {
		return (1);
	}
	ping->pckt.ip = (struct iphdr*)ping->pckt.buf;
}

void    clean_ping(t_ping* ping) {

    free(ping);
}

int main(int argc, char** argv) {
	t_ping*	ping;

	if (argc < 2) {
		exit_error(get_usage_string());
	}
    if (init_ping(ping)) {
        dprintf(STDERR_FILENO, "Error in init_ping\n");
        return (1);
    }
    if (parse_argv(argc, argv, ping)) {

        return (1);
    }
}

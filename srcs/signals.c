//
// Created by Peer De bakker on 11/19/21.
//

#include "ft_ping.h"
#include <signal.h>

void	sigint_handler(int dummy) {
    (void)dummy;
	g_signals.sigint = true;
}

void	sigalarm_handler(int dummy) {
    (void)dummy;
	g_signals.sigalarm = true;
}

void    set_signal_handlers(void) {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        exit_error("Error initializing SIGINT handler");
    }
    if (signal(SIGALRM, sigalarm_handler) == SIG_ERR) {
        exit_error("Error initializing SIGALRM handler");
    }
}

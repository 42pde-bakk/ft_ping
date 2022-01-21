//
// Created by Peer De bakker on 1/19/22.
//

#include <signal.h>
#include <stdio.h>
#include "ft_ping.h"

void	print_signals() {
	dprintf(2, "SIGNALS:\n\t.running=%d\n\t.send=%d\n", g_signals.running, g_signals.send);
}

void    sigalarm_handler(int dummy) {
    (void)dummy;
    g_signals.send = 1;
	print_signals();
}

void    sigint_handler(int dummy) {
    (void)dummy;
    bool should_exit = (bool)!g_signals.running;
    g_signals.running = 0;
    print_signals();
    if (should_exit)
        exit_error("Double sigint received\n");
}

void    set_signal_handlers(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);
}

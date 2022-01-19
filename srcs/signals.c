//
// Created by Peer De bakker on 1/19/22.
//

#include <signal.h>
#include "ft_ping.h"

void    sigalarm_handler(int dummy) {
    (void)dummy;
    g_signals.send = 1;
}

void    sigint_handler(int dummy) {
    (void)dummy;
    g_signals.finito = 1;
}


void    set_signal_handlers(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);
}

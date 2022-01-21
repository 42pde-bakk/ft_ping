//
// Created by Peer De bakker on 1/19/22.
//

#include <signal.h>
#include <stdio.h>
#include "ft_ping.h"

void    sigalarm_handler(int dummy) {
    (void)dummy;
    g_signals.send = 1;
    printf("dummy=%d, g_signals.send=%d\n", dummy, g_signals.send);
}

void    sigint_handler(int dummy) {
    (void)dummy;
    g_signals.finito = 1;
    printf("dummy=%d, g_signals.finito=%d\n", dummy, g_signals.finito);
}


void    set_signal_handlers(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);
}

//
// Created by Peer De bakker on 1/19/22.
//

#include <signal.h>
#include <unistd.h>
#include "ft_ping.h"

void    sigalarm_handler(int dummy) {
    (void)dummy;
    g_signals.send = 1;
}

void    sigint_handler(int dummy) {
    (void)dummy;
    g_signals.running = 0;
    write(1, "\n", sizeof(char));
}

void    sigquit_handler(int dummy) {
    (void)dummy;
    g_signals.sigquit = 1;
    write(1, "\r", sizeof(char));
}

void    set_signal_handlers(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);
    signal(SIGQUIT, sigquit_handler);
    write(1, "\r", sizeof(char));
}

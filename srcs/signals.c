//
// Created by Peer De bakker on 11/19/21.
//

#include "ft_ping.h"

void	sigint_handler(int dummy) {
	g_signals.sigint = true;
}

void	sigalarm_handler(int dummy) {
	g_signals.sigalarm = true;
}
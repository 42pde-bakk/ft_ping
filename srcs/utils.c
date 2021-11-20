//
// Created by Peer De bakker on 11/19/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	exit_error(const char* s) {
	dprintf(STDERR_FILENO, "Error %s\n", s);
	exit(EXIT_FAILURE);
}

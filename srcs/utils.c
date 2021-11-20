//
// Created by Peer De bakker on 11/19/21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

void	exit_error(const char* s) {
	dprintf(STDERR_FILENO, "%s\n", s);
	exit(EXIT_FAILURE);
}

void    fill_timestamp(void* buffer) {
    if (gettimeofday(buffer, NULL) == -1) {
        exit_error("Error filling timestamp");
    }
}

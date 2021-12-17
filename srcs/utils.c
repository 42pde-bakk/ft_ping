#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// Internet checksum (RFC 1071) for error checking, calculated from the ICMP header and data with value 0 substituted for this field.
unsigned short checksum(void *b, int len) {
    unsigned short* buf = b;
    unsigned int    sum;
    unsigned short  result;

    for ( sum = 0; len > 1; len -= 2 ) {
        sum += *buf;
        ++buf;
    }
    if ( len == 1 ) {
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

const char*	get_usage_string(void) {
	const static char* usage_msg = "Usage: ft_ping [-v verbose] [-h help] hostname";

	return (usage_msg);
}

void	exit_error(const char* str) {
	dprintf(STDERR_FILENO, "%s\n", str);
	exit(EXIT_FAILURE);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count == 0 || size == 0) {
		count = 1;
		size = 1;
	}
	ptr = malloc(count * size);
	if (ptr)
		memset(ptr, 0, count * size);
	return (ptr);
}

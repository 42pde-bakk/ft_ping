//
// Created by Peer De bakker on 12/11/21.
//

#include <sys/time.h>
#include <string.h>
#include "ft_ping.h"

uint16_t    in_cksum(u_int16_t *addr, int len)
{

    int         nleft;
    uint16_t    *w;
    uint16_t    answer;
    uint32_t    sum;

    nleft = len;
    sum = 0;
    w = addr;
    answer = 0;

    /*
    **  Our algorithm is simple, using a 32 bit accumulator (sum), we add
    **  sequential 16 bit words to it, and at the end, fold back all the
    **  carry bits from the top 16 bits into the lower 16 bits.
    */

    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1)
    {
        * (unsigned char *) (&answer) = * (unsigned char *) w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

#ifdef __linux__

static void	fill_ip_header(struct iphdr *ip, u_int32_t dest)
{
	ip->version = 4;
	ip->ihl = IP_HDR_SIZE / 4;
	ip->tos = 0;
	ip->tot_len = htons(PACKET_SIZE);
	ip->id = htons(0);
	ip->frag_off = htons(0);
	ip->ttl = 64;
	ip->protocol = IPPROTO_ICMP;
	ip->check = 0;
	ip->saddr = INADDR_ANY;
	ip->daddr = dest;
}

static void	fill_icmp_header(struct icmphdr *ping, int id, int seq)
{
	ping->type = ICMP_ECHO;
	ping->code = 0;
	ping->un.echo.id = htons(id);
	ping->un.echo.sequence = htons(seq);
	ping->checksum = 0;
	ping->checksum = in_cksum((u_int16_t *)ping, ICMP_PAYLOAD_SIZE + ICMP_HDR_SIZE);
}

#elif __APPLE__

static void	fill_ip_header(struct ip *ip, u_int32_t dest)
{
    ip->ip_v = 4;
    ip->ip_hl = IP_HDR_SIZE / 4;
    ip->ip_tos = 0;
    ip->ip_len = PACKET_SIZE;
    ip->ip_id = htons(0);
    ip->ip_off = 0;
    ip->ip_ttl = 64;
    ip->ip_p = IPPROTO_ICMP;
    ip->ip_sum = 0;
    ip->ip_src.s_addr = INADDR_ANY;
    ip->ip_dst.s_addr = dest;
}

static void	fill_icmp_header(struct icmp *ping, int id, int seq)
{
    ping->icmp_type = ICMP_ECHO;
    ping->icmp_code = 0;
    ping->icmp_id = htons(id);
    ping->icmp_seq = htons(seq);
    ping->icmp_cksum = 0;
    ping->icmp_cksum = in_cksum((u_int16_t *) ping, ICMP_PAYLOAD_SIZE + ICMP_HDR_SIZE);
}

#endif

/*
** gen_ip_header
**   provide dest address in network byte order
*/

void			gen_ip_header(void *packet, u_int32_t dest)
{
    fill_ip_header(packet, dest);
}

static void	fill_timestamp(void *buffer)
{
    if (gettimeofday(buffer, NULL) == -1) {
        exit_error("gettimeofday");
    }
}

static void	fill_random_data(void *buffer, size_t size)
{
    memset(buffer, 42, size);
}

void		gen_icmp_msg(void *packet, int seq)
{
    fill_random_data(packet + ICMP_HDR_SIZE, ICMP_PAYLOAD_SIZE);
    fill_timestamp(packet + ICMP_HDR_SIZE + ALIGN_TIMESTAMP);
    fill_icmp_header(packet, 42, seq);
}

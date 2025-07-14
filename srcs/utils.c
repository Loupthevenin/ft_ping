#include "../includes/ft_ping.h"

int	create_socket(t_ping *ping)
{
	int	sockfd;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		perror("socket");
		free_ping(ping);
		exit(1);
	}
	ping->sockfd = sockfd;
	return (0);
}

double	get_rtt_ms(struct timeval *start, struct timeval *end)
{
	long	sec;
	long	usec;

	sec = end->tv_sec - start->tv_sec;
	usec = end->tv_usec - start->tv_usec;
	return (sec * 1000.0) + (usec / 1000.0);
}

void	free_ping(t_ping *ping)
{
	if (ping->target)
	{
		free(ping->target);
		ping->target = NULL;
	}
	if (ping->ip_str)
	{
		free(ping->ip_str);
		ping->ip_str = NULL;
	}
}

void	print_ping(t_ping *ping)
{
	printf("Print_ping: \n");
	printf("  target: %s\n", ping->target);
	printf("  ip_str: %s\n", ping->ip_str);
	printf("  verbose: %d\n", ping->verbose);
	printf("  sockfd: %d\n", ping->sockfd);
	printf("  pid: %d\n", ping->pid);
	printf("  sequence: %d\n", ping->sequence);
	printf("  sent_packets: %d\n", ping->sent_packets);
	printf("  received_packets: %d\n", ping->received_packets);
	printf("  packet_size: %d\n", ping->packet_size);
	printf("  max_count: %d\n", ping->max_count);
	printf("  current_count: %d\n", ping->current_count);
}

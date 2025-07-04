#include "../includes/ft_ping.h"

int	create_socket(t_ping *ping)
{
	ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (!ping->sockfd)
	{
		perror("socket");
		free_ping(ping);
		exit(1);
	}
	return (0);
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
}

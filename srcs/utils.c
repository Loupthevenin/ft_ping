#include "../includes/ft_ping.h"

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
}

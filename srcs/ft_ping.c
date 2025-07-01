#include "../includes/ft_ping.h"

static void	print_ping(t_ping *ping)
{
	printf("Print_ping: \n");
	printf("  target: %s\n", ping->target);
	printf("  ip_str: %s\n", ping->ip_str);
	printf("  verbose: %d\n", ping->verbose);
	printf("  sockfd: %d\n", ping->sockfd);
	printf("  pid: %d\n", ping->pid);
}

static void	init_ping(t_ping *ping)
{
	ping->target = NULL;
	ping->ip_str = NULL;
	ping->verbose = 0;
	ping->sockfd = 0;
	ping->pid = 0;
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

int	main(int argc, char **argv)
{
	t_ping	ping;

	init_ping(&ping);
	if (parse_ping(argc, argv, &ping) != 0)
		return (1);
	if (resolve_target(&ping) != 0)
		return (1);
	print_ping(&ping);
	return (0);
}

#include "../includes/ft_ping.h"

static void	print_usage(void)
{
	printf("Usage: ft_ping [-v] [-?] destination\n");
	printf("  -v : verbose output\n");
	printf("  -? : display this help and exit\n");
}

// TODO: attention leak !;
int	parse_ping(int argc, char **argv, t_ping *ping)
{
	int	i;
	int	is_target;

	i = 1;
	is_target = 0;
	if (argc == 1)
	{
		print_usage();
		exit(64);
	}
	while (i < argc)
	{
		if (strcmp(argv[i], "-v") == 0)
			ping->verbose = 1;
		else if (strcmp(argv[i], "-?") == 0)
		{
			print_usage();
			exit(0);
		}
		else if (argv[i][0] == '-')
		{
			printf("Unknown option: %s\n", argv[i]);
			print_usage();
			exit(0);
		}
		else if (!is_target)
		{
			ping->target = strdup(argv[i]);
			is_target = 1;
		}
		else
		{
			printf("Too many arguments\n");
			print_usage();
			return (1);
		}
		i++;
	}
	return (0);
}

int	resolve_target(t_ping *ping)
{
	struct hostent	*host;
	struct in_addr	*addr;

	host = gethostbyname(ping->target);
	// TODO: attention leak !;
	if (!host)
	{
		fprintf(stderr, "ping: cannot resolve: %s: unknown host\n",
				ping->target);
		exit(68);
	}
	addr = (struct in_addr *)host->h_addr_list[0];
	ping->ip_str = strdup(inet_ntoa(*addr));
	if (!ping->ip_str)
	{
		fprintf(stderr, "ping: memory allocation failed\n");
		exit(1);
	}
	return (0);
}

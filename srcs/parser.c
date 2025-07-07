#include "../includes/ft_ping.h"

static void	print_usage(void)
{
	printf("Usage: ft_ping [-v] [-?] destination\n");
	printf("  -v : verbose output\n");
	printf("  -? : display this help and exit\n");
}

static int	safe_str_to_int(char *str, int *result)
{
	char	*endptr;
	long	val;

	if (!str || *str == '\0')
		return (0);
	for (int i = 0; str[i]; i++)
		if (!isdigit((unsigned char)str[i]))
			return (0);
	val = strtol(str, &endptr, 10);
	if (*endptr != '\0' || val > INT_MAX || val < 0)
		return (0);
	*result = (int)val;
	return (1);
}

int	parse_ping(int argc, char **argv, t_ping *ping)
{
	int	i;

	i = 1;
	if (argc == 1)
	{
		print_usage();
		exit(1);
	}
	while (i < argc)
	{
		if (!strcmp(argv[i], "-v"))
			ping->verbose = 1;
		else if (!strcmp(argv[i], "-?"))
		{
			print_usage();
			free_ping(ping);
			exit(0);
		}
		else if (!strcmp(argv[i], "-c"))
		{
			if (++i >= argc || !isdigit(argv[i][0]))
			{
				fprintf(stderr, "Error: -c requires a number\n");
				free_ping(ping);
				exit(1);
			}
			if (!safe_str_to_int(argv[i], &ping->max_count))
			{
				fprintf(stderr, "Error: invalid value for"
								"-c (must be a positive integer <= %d)\n",
						INT_MAX);
				free_ping(ping);
				exit(1);
			}
		}
		else if (!strcmp(argv[i], "-s"))
		{
			if (++i >= argc || !isdigit(argv[i][0]))
			{
				fprintf(stderr, "Error: -c requires a number\n");
				free_ping(ping);
				exit(1);
			}
			if (!safe_str_to_int(argv[i], &ping->packet_size))
			{
				fprintf(stderr, "Error: invalid value for"
								"-s (must be a positive integer <= %d)\n",
						INT_MAX);
				free_ping(ping);
				exit(1);
			}
			if (ping->packet_size < 0 || ping->packet_size > PACKET_SIZE)
			{
				fprintf(stderr, "Error: invalid packet size\n");
				free_ping(ping);
				exit(1);
			}
		}
		else if (argv[i][0] == '-')
		{
			printf("Unknown option: %s\n", argv[i]);
			print_usage();
			free_ping(ping);
			exit(2);
		}
		else
		{
			if (ping->target)
			{
				fprintf(stderr, "Error: Multiple destinations provided.\n");
				print_usage();
				free_ping(ping);
				exit(1);
			}
			ping->target = strdup(argv[i]);
		}
		i++;
	}
	if (!ping->target)
	{
		fprintf(stderr, "Error: No destination provided.\n");
		print_usage();
		free_ping(ping);
		exit(1);
	}
	return (0);
}

int	resolve_target(t_ping *ping)
{
	struct hostent	*host;
	struct in_addr	*addr;

	host = gethostbyname(ping->target);
	if (!host)
	{
		fprintf(stderr, "ping: cannot resolve: %s: unknown host\n",
				ping->target);
		free_ping(ping);
		exit(2);
	}
	addr = (struct in_addr *)host->h_addr_list[0];
	ping->ip_str = strdup(inet_ntoa(*addr));
	if (!ping->ip_str)
	{
		fprintf(stderr, "ping: memory allocation failed\n");
		free_ping(ping);
		exit(1);
	}
	return (0);
}

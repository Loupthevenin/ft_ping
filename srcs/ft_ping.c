#include "../includes/ft_ping.h"

static void	print_ping(t_ping *ping)
{
	printf("Print_ping: \n");
	printf("  target: %s\n", ping->target);
	printf("  ip_str: %s\n", ping->ip_str);
	printf("  verbose: %d\n", ping->verbose);
	printf("  sockfd: %d\n", ping->sockfd);
	printf("  pid: %d\n", ping->pid);
	printf("  sequence: %d\n", ping->sequence);
}

static void	init_ping(t_ping *ping)
{
	ping->target = NULL;
	ping->ip_str = NULL;
	ping->verbose = 0;
	ping->sockfd = 0;
	ping->pid = 0;
	ping->sequence = 0;
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

static int	create_socket(t_ping *ping)
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

static uint16_t	checksum(void *data, int len)
{
	uint32_t	sum;
	uint16_t	*ptr;

	sum = 0;
	ptr = data;
	while (len > 1)
	{
		sum += *ptr++;
		len -= 2;
	}
	if (len == 1)
		sum += *(uint8_t *)ptr;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return (~sum);
}

static void	init_icmp(t_icmp_echo *icmp, int seq, int pid)
{
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->id = htons(pid);
	icmp->sequence = htons(seq);
	icmp->checksum = 0;
	icmp->checksum = checksum(icmp, sizeof(*icmp));
}

static int	send_ping(t_ping *ping)
{
	ssize_t				sent;
	t_icmp_echo			icmp;
	struct sockaddr_in	addr;

	init_icmp(&icmp, ping->sequence++, ping->pid);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ping->ip_str);
	sent = sendto(ping->sockfd, &icmp, sizeof(icmp), 0,
			(struct sockaddr *)&addr, sizeof(addr));
	if (sent < 0)
	{
		perror("sendto");
		free_ping(ping);
		return (1);
	}
	return (0);
}

static int	receive_ping(t_ping *ping)
{
	(void)ping;
	return (0);
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
	if (create_socket(&ping) != 0)
		return (1);
	// TODO: LOOP HERE;
	if (send_ping(&ping) != 0)
		return (1);
	if (receive_ping(&ping) != 0)
		return (1);
	return (0);
}

#include "../includes/ft_ping.h"

static void	init_ping(t_ping *ping)
{
	ping->target = NULL;
	ping->ip_str = NULL;
	ping->verbose = 0;
	ping->sockfd = 0;
	ping->pid = getpid();
	ping->sequence = 0;
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
	char				buffer[PACKET_SIZE];
	ssize_t				bytes;
	struct sockaddr_in	from;
	socklen_t			from_len;
	struct ip			*ip_hdr;
	t_icmp_echo			*icmp_hdr;
	int					ip_header_len;

	from_len = sizeof(from);
	bytes = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr *)&from, &from_len);
	if (bytes < 0)
	{
		perror("recvfrom");
		free_ping(ping);
		return (1);
	}
	// Lire l'entête IP
	ip_hdr = (struct ip *)buffer;
	ip_header_len = ip_hdr->ip_hl << 2;
	// Lire l'entête ICMP
	icmp_hdr = (t_icmp_echo *)(buffer + ip_header_len);
	if (icmp_hdr->type != 0 || icmp_hdr->id != htons(ping->pid))
	{
		if (ping->verbose)
			printf("Received non-reply or wrong id\n");
		free_ping(ping);
		return (1);
	}
	printf("%zd bytes from %s: icmp_seq=%d ttl=%d\n", bytes - ip_header_len,
			inet_ntoa(from.sin_addr), ntohs(icmp_hdr->sequence),
			ip_hdr->ip_ttl);
	return (0);
}

static int	run_ping_loop(t_ping *ping)
{
	while (1)
	{
		if (send_ping(ping) != 0)
			return (1);
		if (receive_ping(ping) != 0)
			return (1);
		sleep(1);
	}
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
	if (run_ping_loop(&ping) != 0)
		return (1);
	return (0);
}

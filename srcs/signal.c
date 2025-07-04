#include "../includes/ft_ping.h"

static t_ping	*get_ping_instance(t_ping *ping)
{
	static t_ping	*ref = NULL;

	if (ping != NULL)
		ref = ping;
	return (ref);
}

void	set_ping_instance(t_ping *ping)
{
	get_ping_instance(ping);
}

void	handle_sigint(int sig)
{
	t_ping	*ping;
	double	loss;

	(void)sig;
	ping = get_ping_instance(NULL);
	if (!ping)
	{
		free_ping(ping);
		exit(1);
	}
	loss = 0.0;
	if (ping->sent_packets > 0)
		loss = 100.0 * (ping->sent_packets - ping->received_packets)
			/ ping->sent_packets;
	printf("\n--- %s ping statistics ---\n", ping->target);
	// TODO: add time ms:
	printf("%d packets transmitted, %d received, %.1f%% packet loss\n",
			ping->sent_packets,
			ping->received_packets,
			loss);
	free_ping(ping);
	exit(0);
}

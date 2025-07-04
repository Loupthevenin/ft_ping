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

static void	print_sigint(t_ping *ping)
{
	double			loss;
	double			total_time_ms;
	struct timeval	end_time;
	double			avg;

	loss = 0.0;
	if (ping->sent_packets > 0)
		loss = 100.0 * (ping->sent_packets - ping->received_packets)
			/ ping->sent_packets;
	gettimeofday(&end_time, NULL);
	total_time_ms = get_rtt_ms(&ping->start_time, &end_time);
	printf("\n--- %s ping statistics ---\n", ping->target);
	printf("%d packets transmitted, %d received, %.1f%% packet loss time "
			"%.0fms\n",
			ping->sent_packets,
			ping->received_packets,
			loss,
			total_time_ms);
	if (ping->received_packets > 0)
	{
		avg = ping->total_rtt / ping->received_packets;
		printf("rtt min/avg/max = %.3f/%.3f/%.3f ms\n", ping->min_rtt, avg,
				ping->max_rtt);
	}
}

void	handle_sigint(int sig)
{
	t_ping	*ping;

	(void)sig;
	ping = get_ping_instance(NULL);
	if (!ping)
	{
		free_ping(ping);
		exit(1);
	}
	print_sigint(ping);
	free_ping(ping);
	exit(0);
}

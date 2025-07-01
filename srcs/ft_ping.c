#include "../includes/ft_ping.h"

int	main(int argc, char **argv)
{
	t_ping	ping;

	if (parse_ping(argc, argv, &ping) != 0)
		return (1);
	return (0);
}

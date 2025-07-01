#ifndef FT_PING_H
# define FT_PING_H

# include "stdio.h"

typedef struct s_ping
{
	char	*target;
	char	*ip_str;
	int		verbose;
	int		sockfd;
	int		pid;
}			t_ping;

int			parse_ping(int argc, char **argv, t_ping *ping);

#endif

#ifndef FT_PING_H
# define FT_PING_H

# include "arpa/inet.h"
# include "netdb.h"
# include "netinet/in.h"
# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "sys/socket.h"
# include "sys/time.h"
# include "unistd.h"

typedef struct s_ping
{
	char	*target;
	char	*ip_str;
	int		verbose;
	int		sockfd;
	int		pid;
}			t_ping;

int			parse_ping(int argc, char **argv, t_ping *ping);
int			resolve_target(t_ping *ping);

// Utils;
void		free_ping(t_ping *ping);

#endif

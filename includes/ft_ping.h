#ifndef FT_PING_H
# define FT_PING_H

# include "arpa/inet.h"
# include "netdb.h"
# include "netinet/in.h"
# include "netinet/ip.h"
# include "netinet/ip_icmp.h"
# include "signal.h"
# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "sys/socket.h"
# include "sys/time.h"
# include "unistd.h"

# define PACKET_SIZE 1024

typedef struct s_ping
{
	char			*target;
	char			*ip_str;
	int				verbose;
	int				sockfd;
	int				pid;
	int				sequence;
	struct timeval	send_time;
	int				sent_packets;
	int				received_packets;
}					t_ping;

typedef struct s_icmp_echo
{
	uint8_t			type;
	uint8_t			code;
	uint16_t		checksum;
	uint16_t		id;
	uint16_t		sequence;
}					t_icmp_echo;

int					parse_ping(int argc, char **argv, t_ping *ping);
int					resolve_target(t_ping *ping);

// Signal
void				set_ping_instance(t_ping *ping);
void				handle_sigint(int sig);

// Utils;
int					create_socket(t_ping *ping);
void				free_ping(t_ping *ping);

// Debug;
void				print_ping(t_ping *ping);

#endif

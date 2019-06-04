#ifndef IRC_IRC_H
#define IRC_IRC_H

/** Includes **/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
/** Data **/

typedef struct	s_client {
	int		fildes;
	char	name[20];
	size_t	len;

}				t_client;

/** Prototype **/

void create_server(unsigned short port);

#endif //IRC_IRC_H

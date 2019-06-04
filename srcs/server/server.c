#include <fcntl.h>
#include "../../incs/irc.h"

#define MAX_MSG 512
#define MAX_NAME 20

void format_string(char **buffer, t_client *client, int nbytes);

void
send_active_client(t_client *clients[1024], char *buffer);

static int read_from_client(int fildes, char **buffer) {
	return recv(fildes, *buffer, MAX_MSG, 0);
}

static t_client *accept_connection(int sock) {
	t_client *client = NULL;
	int fildes = 0;
	if ((fildes = accept(sock, NULL, NULL)) > 0) {
		client = (t_client *)malloc(sizeof(t_client));
		printf("accept %d\n", fildes);
		client->fildes = fildes;
		write(fildes, "Bonjour\n", 8);
		printf("%s\n", "New connection");
	}

	return client;
}

static int init_server(unsigned short port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	fcntl(sock, F_SETFL, O_NONBLOCK);
//
	struct sockaddr_in sin;

	bzero(&sin, sizeof(sin));
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if (bind(sock, (const struct sockaddr *) &sin, sizeof(sin)) == -1)
		perror("bind");

	if (listen(sock, 20) == -1)
		perror("listen");
	return sock;
}

void create_server(unsigned short port) {
	struct timeval timeval = {1 , 0};

	char *buffer = malloc(MAX_MSG + MAX_NAME + 2);
	fd_set active_fildes, read_fildes;
	t_client *clients[FD_SETSIZE] = {0};
	t_client *temp;
	int sock;
	FD_ZERO(&active_fildes);
	sock = init_server(port);
	FD_SET(sock, &active_fildes);
	int max_fd = sock + 1;
	printf("Open server on Socket %d\n", sock);
	while (true) {
		FD_ZERO(&read_fildes);
		memcpy(&read_fildes, &active_fildes, sizeof(active_fildes));
		int ret;
		if ((ret = select(max_fd + 1, &read_fildes, NULL, NULL, &timeval)) < 0) {
			perror("select");
			exit(1);
		}
		printf("-- Loop -- %d\n", ret);
		for (int index = 0; index < max_fd + 1; ++index) {
			if (FD_ISSET(index, &read_fildes)) {
				printf("-- FD_ISSET on %d --\n", index);

				if (index == sock) {
					if ((temp = accept_connection(sock)) != NULL) {
						printf("Server accept new connection [%d]\n", temp->fildes);
						FD_SET(temp->fildes, &active_fildes);
						clients[temp->fildes] = temp;
						strcpy(clients[temp->fildes]->name, "coucou");
						clients[temp->fildes]->len = 6;
						max_fd = temp->fildes > max_fd ? temp->fildes : max_fd;
					}
				} else {
					printf("read from client\n");
					if (clients[index]) {
						printf("bzero\n");
						bzero(buffer, MAX_MSG + MAX_NAME + 2);
						printf("read_from_client\n");
						int nbytes = read_from_client(clients[index]->fildes, &buffer);
						printf("format_string\n");
						format_string(&buffer, clients[index], nbytes);
						printf("send_active_client\n");
						send_active_client(clients, buffer);
					}
				}
			}
		}
	}
}

void
send_active_client(t_client *clients[1024], char *buffer) {
	for (int index = 0; index < FD_SETSIZE; ++index) {
		if (clients[index])
			send(clients[index]->fildes, buffer, MAX_MSG + MAX_NAME + 2, 0);
	}

}

void format_string(char **buffer, t_client *client, int nbytes) {
	strncpy(*buffer + MAX_NAME + 2, *buffer, nbytes);
	memset(*buffer, 32, MAX_NAME + 2);
	(*buffer)[0] = '[';
	(*buffer)[MAX_NAME - 1] = ']';
	strncpy(*buffer + ((MAX_NAME + 2) / 2 - client->len / 2), client->name, client->len);
}




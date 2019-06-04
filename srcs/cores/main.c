#include "../../incs/irc.h"

#define SERVER_PORT 8000
#define SERVER_PARAMETER "server"
#define CLIENT_PARAMETER "client"

int main(int argc, char *argv[]) {

	printf("%d\n", argc);

	if (argc > 1) {
		if (strcmp(argv[1], SERVER_PARAMETER) == 0)
			create_server(argc == 2 ? SERVER_PORT : atoi(argv[2]));
	}

	return 0;
}
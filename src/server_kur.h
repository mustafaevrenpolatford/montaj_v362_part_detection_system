
#ifndef _SERVER_KUR_
#define _SERVER_KUR_

#include"includelar.h"
extern int pvs_Server_PORT  = 3334;

int server_kur(int *server_fd, struct sockaddr_in *address_ptr, int addrlen, int PORT)
{
	int server_fd_temp;
	int opt = 1;
	// Creating socket file descriptor
	server_fd_temp = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd_temp == 0)
	{
		syslog(LOG_INFO, "socket failed\n");
		return -1;
	}

	if (setsockopt(server_fd_temp, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		syslog(LOG_INFO, "setsockopt\n");
		return -1;
	}

	memset(address_ptr, '0', addrlen);
	address_ptr->sin_family = AF_INET;
	address_ptr->sin_addr.s_addr = INADDR_ANY;
	address_ptr->sin_port = htons(PORT);

	if (bind(server_fd_temp, (struct sockaddr *)address_ptr, addrlen) < 0)
	{
		syslog(LOG_INFO, "bind failed\n");
		return -1;
	}

	if (listen(server_fd_temp, 3) < 0)
	{
		syslog(LOG_INFO, "listen\n");
		return -1;
	}
	*server_fd = server_fd_temp;

	return 0;
}

#endif
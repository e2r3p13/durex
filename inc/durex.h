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

typedef struct
{
	int confd;
	int is_logged;
	int is_typing;
	int shell_pid;
}	t_cli;

int ft_daemon(void);

#endif

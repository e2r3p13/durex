/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bcrypt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:30:24 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/16 18:31:12 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int bcrypt_checkpass(const char *pass, const char *goodhash)
{
	return (strcmp(pass, goodhash) == 0);
}

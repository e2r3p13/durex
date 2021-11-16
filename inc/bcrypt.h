/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bcrypt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:11:25 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/16 18:12:58 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BCRYPT_H
#define BCRYPT_H

/*
 * This function is the only one we'll use as the pass hash is already harcoded,
 * thus we only need to check given ones.
*/
int bcrypt_checkpass(const char *pass, const char *goodhash);

#endif

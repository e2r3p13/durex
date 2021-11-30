/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lure.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 19:04:16 by bccyv             #+#    #+#             */
/*   Updated: 2021/11/30 02:26:18 by bccyv            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LURE_H
#define LURE_H

typedef struct
{
	int x;
	int y;
}	t_coords;

typedef struct
{
	unsigned char **data;
	size_t h;
	size_t w;
	t_coords player;
}	t_map;

typedef enum
{
	UP = 1,
	RIGHT,
	DOWN,
	LEFT
}	e_move;

#endif

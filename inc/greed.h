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

#define WALL_CHR '#'
#define PLAYER_CHR '@'
#define MAP_H 16
#define MAP_W 32

typedef struct
{
	int x;
	int y;
}	t_coords;

typedef struct
{
	unsigned char **data;
	int h;
	int w;
	t_coords player;
}	t_map;

typedef enum
{
	UP = 1,
	RIGHT,
	DOWN,
	LEFT
}	e_move;

int		greed();

t_map	*map_create(int w, int h);
void	map_generate(t_map *map);
void	map_draw(t_map *map, int score);
void	*map_free(t_map *map);

int		can_move_up(t_map *map);
int		can_move_down(t_map *map);
int		can_move_left(t_map *map);
int		can_move_right(t_map *map);
e_move	move_get();
int		move_perform(t_map *map, e_move move);

char	*ft_strcat(char *dst, const char *src);
int		ft_strnf(int fd, const char *s);

#endif

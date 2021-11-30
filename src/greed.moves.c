#include <unistd.h>
#include <string.h>
#include <greed.h>

/*
 * Reads stdin until a valid move arrives.
 * Valid moves are only arrow keys.
*/
e_move move_get()
{
	char buf[5] = {0};

	while (1)
	{
		memset(buf, 0, 5);
		if (read(0, buf, 4) < 3)
			continue ;
		if (buf[0] == '\033' && buf[1] == '[')
		{
			switch (buf[2])
			{
				case 'A': return (UP);
				case 'B': return (DOWN);
				case 'C': return (RIGHT);
				case 'D': return (LEFT);
			}
		}
	}
	return (UP); // Should never be called
}

/*
 * The following can_move_xxx functions first checks that the player isn't on
 * a map boundary and that there's a digit immediately next to him on that
 * direction.
 * It then makes sure that the destination slot is also on the map and is also
 * a digit.
 * Returns 1 if the move is legal, the distance otherwise,
 * (to avoid double computation in perform_move).
*/

int can_move_up(t_map *map)
{
    if (map->player.y > 0 && map->data[map->player.y - 1][map->player.x] != ' ')
	{
		int dist = map->data[map->player.y - 1][map->player.x] - '0';
		if (map->player.y - dist >= 0 && map->data[map->player.y - dist][map->player.x] - '0' > 0)
			return (dist);
	}
    return (0);
}

int can_move_down(t_map *map)
{
    if (map->player.y < map->h - 2 && map->data[map->player.y + 1][map->player.x] != ' ')
	{
		int dist = map->data[map->player.y + 1][map->player.x] - '0';
		if (map->player.y + dist < map->h && map->data[map->player.y + dist][map->player.x] - '0' > 0)
			return (dist);
	}
    return (0);
}

int can_move_left(t_map *map)
{
    if (map->player.x > 0 && map->data[map->player.y][map->player.x - 1] != ' ')
	{
		int dist = map->data[map->player.y][map->player.x - 1] - '0';
		if (map->player.x - dist >= 0 && map->data[map->player.y][map->player.x - dist] - '0' > 0)
			return (dist);
	}
    return (0);
}

int can_move_right(t_map *map)
{
    if (map->player.x < map->w - 2 && map->data[map->player.y][map->player.x + 1] != ' ')
	{
		int dist = map->data[map->player.y][map->player.x + 1] - '0';
		if (map->player.x + dist < map->w && map->data[map->player.y][map->player.x + dist] - '0' > 0)
			return (dist);
	}
	return (0);
}

/*
 * Tries to perform the @move move on the @map map.
 * Returns the number of earned points if the move has been done,
 * 0 if the move isn't allowed.
*/
int move_perform(t_map *map, e_move move)
{
	int dist;

	switch (move)
	{
		case UP:
            dist = can_move_up(map);
			for (int i = 0; i < dist; i++)
			{
				if (map->data[map->player.y - i][map->player.x] != ' ')
					map->data[map->player.y - i][map->player.x] = ' ';
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.y -= dist;
			map->data[map->player.y][map->player.x] = PLAYER_CHR;
			break ;
		case RIGHT:
			dist = can_move_right(map);
			for (int i = 0; i < dist; i++)
			{
				if (map->data[map->player.y][map->player.x + i] != ' ')
					map->data[map->player.y][map->player.x + i] = ' ';
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.x += dist;
			map->data[map->player.y][map->player.x] = PLAYER_CHR;
			break ;
		case DOWN:
            dist = can_move_down(map);
			for (int i = 0; i < dist; i++)
			{
				if (map->data[map->player.y + i][map->player.x] != ' ')
					map->data[map->player.y + i][map->player.x] = ' ';
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.y += dist;
			map->data[map->player.y][map->player.x] = PLAYER_CHR;
			break ;
		case LEFT:
            dist = can_move_left(map);
			for (int i = 0; i < dist; i++)
			{
				if (map->data[map->player.y][map->player.x - i] != ' ')
					map->data[map->player.y][map->player.x - i] = ' ';
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.x -= dist;
			map->data[map->player.y][map->player.x] = PLAYER_CHR;
			break ;
	}
	return (dist);
}

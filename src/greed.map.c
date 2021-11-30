#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <greed.h>

/*
 * Global used to set digits color. Index 0 is for player color.
*/
const char *color_codes[10] = {
	[0] = "\033[107m",
	[1] = "\033[34m",
	[2] = "\033[94m",
	[3] = "\033[96m",
	[4] = "\033[92m",
	[5] = "\033[93m",
	[6] = "\033[33m",
	[7] = "\033[31m",
	[8] = "\033[91m",
	[9] = "\033[95m",
};

/*
 * Those lines will b printed on the right side of the map, at their nth index
 * line.
*/
const char *sidenotes[MAP_H] = {
    [0] = "\t\033[32;1m[GREED - BY LFALKAU]\033[0m",
    [2] = "\t\033[0;1mGOAL:\033[0m",
    [3] = "\t   Clean the board as much as you can!",
    [4] = "\t   When you move in a direction bla bla bla...",
    [6] = "\t\033[0;1mCONTROLS:\033[0m",
    [7] = "\t   Use your arrows to move on the greed",
    [8] = "\t   Hit Ctrl + C to quit",
    [10] = "\t\033[0;1mSCORE: \033[35m%d\033[0m",
};

/*
 * Freees a map structure
*/
void *map_free(t_map *map)
{
    if (map->data)
    {
        for (int i = 0; i < map->h; i++)
        {
            if (map->data[i])
                free(map->data[i]);
            else
                break ;
        }
        free(map->data);
    }
    free(map);
    return (NULL);
}

/*
 * Allocates a 2D map of @w by @h.
 * Returns the map struct on success, NULL on error.
*/
t_map *map_create(int w, int h)
{
	t_map *map;

	map = malloc(sizeof(t_map));
	if (map == NULL)
		return (NULL);
	map->h = h;
	map->w = w;
	map->player.x = 0;
	map->player.y = 0;
	map->data = malloc(sizeof(char *) * (h + 1));
	if (map->data == NULL)
		return (map_free(map));
	for (int i = 0; i < h; i++)
	{
		map->data[i] = malloc(sizeof(char) * (w + 1));
		if (map->data[i] == NULL)
		     return (map_free(map));
		map->data[i][w] = '\0';
	}
	map->data[h] = NULL;
	return (map);
}

/*
 * Initializes an allocated map
*/
void map_generate(t_map *map)
{
	srand(time(NULL));

	// Fills the whole map with random digits (1-9);
	for (int y = 0; y < map->h; y++)
		for (int x = 0; x < map->w; x++)
			map->data[y][x] = rand() % 9 + '1';

	// Sets the player position randomly
	map->player.x = rand() % map->w;
	map->player.y = rand() % map->h;
	map->data[map->player.y][map->player.x] = PLAYER_CHR;
}

static void print_line(const unsigned char c, size_t length)
{
    for (size_t i = 0; i < length; i++)
        printf("%c", c);
    printf("\n");
}

/*
 * Prints the @map map on stdout
*/
void map_draw(t_map *map, int score)
{
	printf("\e[1;1H\e[2J"); // clears the terminal
	print_line(WALL_CHR, map->w * 2 + 3);
	for (int y = 0; y < map->h; y++)
	{
		printf("%c ", WALL_CHR);
		for (int x = 0; x < map->w; x++)
		{
            unsigned char digit_chr = map->data[y][x];
			int color_code_index = digit_chr == PLAYER_CHR ? 0 : digit_chr - '0';
			printf("%s%c\033[0m ", color_code_index >= 0 ? color_codes[color_code_index] : "", digit_chr);
		}
        printf("%c", WALL_CHR);

        if (sidenotes[y])
            printf(sidenotes[y], score);
		printf("\n");
	}
    print_line('#', map->w * 2 + 3);
}

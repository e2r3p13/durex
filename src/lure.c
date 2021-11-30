#include <stdio.h>
#include <lure.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

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

t_map *map_create(size_t w, size_t h)
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
	{
		free(map);
		return (NULL);
	}
	for (size_t i = 0; i < h; i++)
	{
		map->data[i] = malloc(sizeof(char) * (w + 1));
		if (map->data[i] == NULL)
		{
			while (i--)
				free(map->data[i]);
			free(map->data);
			free(map);
			return (NULL);
		}
		map->data[i][w] = '\0';
	}
	map->data[h] = NULL;
	return (map);
}

void map_generate(t_map *map)
{
	srand(time(NULL));

	// Fills the whole map with random digits;
	for (size_t y = 0; y < map->h; y++)
		for (size_t x = 0; x < map->w; x++)
			map->data[y][x] = rand() % 9 + '1';

	// Sets the player position randomly
	map->player.x = rand() % map->w;
	map->player.y = rand() % map->h;
	map->data[map->player.y][map->player.x] = '#';
}

void map_draw(t_map *map, int score);

int game_over(t_map *map)
{
	int dist;

	// up
	if (map->player.y > 0 && map->data[map->player.y - 1][map->player.x] != ' ')
	{
		dist = map->data[map->player.y - 1][map->player.x] - '0';
		if (map->player.y - dist >= 0 && map->data[map->player.y - dist][map->player.x] - '0' > 0)
			return (0);
	}
	// left
	if (map->player.x > 0 && map->data[map->player.y][map->player.x - 1] != ' ')
	{
		dist = map->data[map->player.y][map->player.x - 1] - '0';
		if (map->player.x - dist >= 0 && map->data[map->player.y][map->player.x - dist] - '0' > 0)
			return (0);
	}
	// down
	if (map->player.y < map->h - 2 && map->data[map->player.y + 1][map->player.x] != ' ')
	{
		dist = map->data[map->player.y + 1][map->player.x] - '0';
		if (map->player.y + dist < map->h && map->data[map->player.y + dist][map->player.x] - '0' > 0)
			return (0);
	}
	// right
	if (map->player.x < map->w - 2 && map->data[map->player.y][map->player.x + 1] != ' ')
	{
		dist = map->data[map->player.y][map->player.x + 1] - '0';
		if (map->player.x + dist < map->w && map->data[map->player.y][map->player.x + dist] - '0' > 0)
			return (0);
	}
	return (1);
}

void map_draw(t_map *map, int score)
{
	printf("\e[1;1H\e[2J"); // clears the terminal
	wprintf(L"%lc", (wchar_t)0x250f);
	for (size_t i = 0; i < map->w * 2 + 1; i++)
		wprintf(L"%lc", (wchar_t)0x2501);
	wprintf(L"%lc\n", (wchar_t)0x2513);
	for (size_t y = 0; y < map->h; y++)
	{
		wprintf(L"%lc ", (wchar_t)0x2503);
		for (size_t x = 0; x < map->w; x++)
		{
			int n = map->data[y][x] == '#' ? 0 : map->data[y][x] - '0';
			printf("%s%c\033[0m ", n >= 0 ? color_codes[n] : "", map->data[y][x]);
			if (x == map->w - 1)
				wprintf(L"%lc", (wchar_t)0x2503);
		}
		if (y == 0)
			printf("\t\033[32;1m[GREED - BY LFALKAU]\033[0m\n");
		else if (y == 2)
			printf("\t\033[0;1mGOAL:\033[0m\n");
		else if (y == 3)
			printf("\t   Clean the board as much as you can!\n");
		else if (y == 4)
			printf("\t   When you move in a direction bla bla bla...\n");
		else if (y == 6)
			printf("\t\033[0;1mCONTROLS:\033[0m\n");
		else if (y == 7)
			printf("\t   Use your arrows to move on the greed\n");
		else if (y == 8)
			printf("\t   Hit Ctrl + C to quit\n");
		else if (y == 10)
			printf("\t\033[0;1mSCORE: \033[35m%d\033[0m\n", score);
		else
			printf("\n");
	}
	wprintf(L"%lc", (wchar_t)0x2517);
	for (size_t i = 0; i < map->w * 2 + 1; i++)
		wprintf(L"%lc", (wchar_t)0x2501);
	wprintf(L"%lc\n", (wchar_t)0x251b);
}

e_move get_input()
{
	char buf[5] = {0};
	e_move move;

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

int perform_move(t_map *map, e_move move)
{
	int dist;
	int points = 0;

	switch (move)
	{
		case UP:
			if (map->player.y == 0)
				return (0);
			dist = map->data[map->player.y - 1][map->player.x] - '0';
			if (dist < 0 || map->player.y < dist || map->data[map->player.y - dist][map->player.x] == ' ')
				return (0);
			for (size_t i = 0; i < dist; i++)
			{
				if (map->data[map->player.y - i][map->player.x] != ' ')
				{
					map->data[map->player.y - i][map->player.x] = ' ';
					points++;
				}
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.y -= dist;
			map->data[map->player.y][map->player.x] = '#';
			break ;
		case RIGHT:
			if (map->player.x == map->w - 1)
				return (0);
			dist = map->data[map->player.y][map->player.x + 1] - '0';
			if (dist < 0 || map->player.x + dist >= map->w || map->data[map->player.y][map->player.x + dist] == ' ')
				return (0);
			for (size_t i = 0; i < dist; i++)
			{
				if (map->data[map->player.y][map->player.x + i] != ' ')
				{
					map->data[map->player.y][map->player.x + i] = ' ';
					points++;
				}
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.x += dist;
			map->data[map->player.y][map->player.x] = '#';
			break ;
		case DOWN:
			if (map->player.y == map->h - 1)
				return (0);
			dist = map->data[map->player.y + 1][map->player.x] - '0';
			if (dist < 0 || map->player.y + dist >= map->h || map->data[map->player.y + dist][map->player.x] == ' ')
				return (0);
			for (size_t i = 0; i < dist; i++)
			{
				if (map->data[map->player.y + i][map->player.x] != ' ')
				{
					map->data[map->player.y + i][map->player.x] = ' ';
					points++;
				}
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.y += dist;
			map->data[map->player.y][map->player.x] = '#';
			break ;
		case LEFT:
			if (map->player.x == 0)
				return (0);
			dist = map->data[map->player.y][map->player.x - 1] - '0';
			if (dist < 0 || map->player.x < dist || map->data[map->player.y][map->player.x - dist] == ' ')
				return (0);
			for (size_t i = 0; i < dist; i++)
			{
				if (map->data[map->player.y][map->player.x - i] != ' ')
				{
					map->data[map->player.y][map->player.x - i] = ' ';
					points++;
				}
			}
			map->data[map->player.y][map->player.x] = ' ';
			map->player.x -= dist;
			map->data[map->player.y][map->player.x] = '#';
			break ;
	}
	return (points);
}

int lure()
{
	t_map *map;
	int score = 0;

	map = map_create(32, 16);
	if (map == NULL)
	{
		printf("Error while generating the map\n");
		return (1);
	}
	map_generate(map);
	map_draw(map, score);

	while (!game_over(map))
	{
		e_move move = get_input();
		score += perform_move(map, move);
		map_draw(map, score);
	}
	printf("\n\033[31;1m[GAME OVER]\033[0m\nFinal score: \033[35m%d\033[0m\n\n", score);
	return (0);
}

int toggle_echo(int state)
{
	struct termios t;

	if (tcgetattr(1, &t) < 0)
		return (-1);
	if (state)
	{
		t.c_lflag |= ICANON; // disable canonical mode
		t.c_lflag |= ECHO; // turn on echo
		printf("\e[?25h"); // show cursor
	}
	else
	{
		t.c_lflag &= ~ICANON; // enable canonical mode
		t.c_lflag &= ~ECHO; // turn off echo
		printf("\e[?25l"); // hide cursor
	}
	fflush(stdout);
	return (tcsetattr(1, TCSANOW, &t));
}

void sighandler(int sig)
{
	toggle_echo(1);
	exit(128 + sig);
}

int main()
{
	signal(SIGINT, &sighandler);

	if (toggle_echo(0) < 0)
		return (1);

	setlocale(LC_CTYPE, "");
	lure();

	if (toggle_echo(1) < 0)
		return (1);

	return (0);
}

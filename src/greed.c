#include <stdio.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <greed.h>

/*
 * Returns 1 if the game is over, 0 otherwise.
*/
int game_over(t_map *map)
{
	return
	(
		!can_move_up(map) &&
		!can_move_down(map) &&
		!can_move_left(map) &&
		!can_move_right(map)
	);
}

/*
 * Greed core function.
*/
int greed_start()
{
	t_map *map;
	int score = 0;

	map = map_create(MAP_W, MAP_H);
	if (map == NULL)
	{
		printf("Error while generating the map\n");
		return (1);
	}
	map_generate(map);
	map_draw(map, score);

	// Game main loop
	while (!game_over(map))
	{
		e_move move = move_get();
		score += move_perform(map, move);
		map_draw(map, score);
	}

	printf("\n\033[31;1m[GAME OVER]\033[0m\nFinal score: \033[35m%d\033[0m\n\n", score);
	map_free(map);
	return (0);
}

/*
 * Turns terminal echoing on (if state == 1) of off (state == 0).
 * It also shows/hides the cursor and enable/disable the canonical mode.
 * Returns 0 on success, -1 on error.
*/
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

/*
 * SIGINT handler, used to restore the terminal
*/
void sighandler(int sig)
{
	toggle_echo(1);
	exit(128 + sig);
}

/*
 * Greed entry function.
 * Sets the terminal mode and sighandlers before the game starts.
*/
int greed()
{
	signal(SIGINT, &sighandler);

	if (toggle_echo(0) < 0)
		return (1);

	greed_start();

	if (toggle_echo(1) < 0)
		return (1);

	return (0);
}

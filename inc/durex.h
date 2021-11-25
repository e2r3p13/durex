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

int		lure();
int		ft_daemonize(void);

void	debug(const char *fmt, ...);

#endif

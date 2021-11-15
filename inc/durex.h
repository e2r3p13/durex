#ifndef DUREX_H
#define DUREX_H

typedef struct
{
	int confd;
	int is_typing;
}	t_cli;

int	payload(void);
int	daemonize(void);

#endif

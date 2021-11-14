#include <durex.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

int main(void)
{
	int fd;
	const char program[] = ""

	fprintf(stdout, "lfalkau\n");
	fd = open(BPATH NAME, O_WRONLY | O_CREAT, S_IXUSR);
	if (fd < 0)
	{
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return (0);
	}
	dprintf(fd, "%s\n", program);
	return (0);
}

#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

static int close_all_filedes()
{
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) < 0)
		return (-1);
	for (size_t i = 3; i < limit.rlim_cur; i++)
		close(i);
    return (0);
}

static int reset_signal()
{
    sigset_t new_set;

    for (size_t i = 0; i < _NSIG; i++)
		signal(i, SIG_DFL);
    sigemptyset(&new_set);
	if (sigprocmask(SIG_SETMASK, &new_set, NULL) < 0)
        return (-1);
    return (0);
}

int redirect_std_filedes(const char *dstpath)
{
    int fd;

    fd = open(dstpath, O_RDWR);
    if (fd < 0)
        return (-1);
    for (int i = 0; i < 3; i++)
		dup2(fd, i);
	close(fd);
    return (0);
}

/*
 * This code is only called from the final daemon process.
 * It redirects standard input/output/error to /dev/null, resets the umask to 0,
 * sets the current working directory to the root, and checks / update the
 * pidfile in /run.
*/
int daemon_core()
{
    int pidfile_fd;
    char buf[16] = {0};

    if (redirect_std_filedes("/dev/null") < 0)
        return (-1);
    umask((mode_t){0});
    chdir("/");
    pidfile_fd = open("/run/Durex.pid", O_RDWR | O_CREAT, 00600);
    if (pidfile_fd < 0)
        return (-1);
    if (read(pidfile_fd, buf, 15) > 0)
    {
        int oldpid = atoi(buf);
        if (kill(oldpid, 0) < 0)
        {
            lseek(pidfile_fd, 0, SEEK_SET);
            dprintf(pidfile_fd, "%d\n", getpid());
        }
    }
    else
    {
        dprintf(pidfile_fd, "%d\n", getpid());
    }
    close(pidfile_fd);
    return (0);
}

/*
 * Here, we'll use an old style daemon, to be sysV compatible.
 * See https://www.man7.org/linux/man-pages/man7/daemon.7.html for more details.
*/
int ft_daemon()
{
	int	pid[2];

	if (close_all_filedes() < 0 || reset_signal() < 0)
		return (-1);

	pid[0] = fork();
	if (pid[0] < 0)
		return (-1);
	if (pid[0] == 0)
	{
		setsid();
		pid[1] = fork();
		if (pid[1] != 0)
			exit(EXIT_SUCCESS);
		return (daemon_core());
	}
    else
    {
        // To make sure the initisation of the daemon process has ended.
        // Should be done with an unnamed pipe and an effective notify system.
        usleep(100000);
        exit(EXIT_SUCCESS);
    }
	return (0);
}

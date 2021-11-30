/*
 * Here, we'll use an old style daemon, to be sysV compatible.
 * See https://www.man7.org/linux/man-pages/man7/daemon.7.html for more details.
*/
int ft_deamon()
{
	struct rlimit limit;
	sigset_t new_set;
	int	pid[2];

	// Close all file descriptors
	if (getrlimit(RLIMIT_NOFILE, &limit) < 0)
		return (-1);
	for (size_t i = 3; i < limit.rlim_cur; i++)
		close(i);

	// Resetting all signals
	for (size_t i = 0; i < _NSIG; i++)
		signal(i, SIG_DFL);

    // Resetting signal mask
	sigemptyset(&new_set);
	if (sigprocmask(SIG_SETMASK, &new_set, NULL) < 0)
        return (-1);

	pid[0] = fork();
	if (pid[0] != 0)
		exit(EXIT_SUCCESS);
	if (pid[0] == 0)
	{
		if (setsid() < 0)
			exit(EXIT_SUCCESS);

		pid[1] = fork();
		if (pid[1] != 0)
			exit(EXIT_SUCCESS);

		// Daemon process
		int fd = open("/dev/null", O_RDWR);
		if (fd < 0)
			exit(EXIT_FAILURE);
		for (int i = 0; i < 3; i++)
			dup2(fd, i);
		close(fd);

		umask((mode_t){0});
		chdir("/");

	}

	return (0);
}

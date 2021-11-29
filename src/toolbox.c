#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

void debug(const char *fmt, ...)
{
	va_list args;

	#ifdef DEBUG
	    va_start(args, fmt);
	    vprintf(fmt, args);
	    va_end(args);
	#else
		(void)fmt;
		(void)args;
	#endif
}

int ft_strlen(const char *s)
{
	int i = 0;

	while (s[i])
		i++;

	return (i);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	while (s1[i] && s2[i] && i < n && s1[i] == s2[i])
		i++;
	return (i == n ? 0 : ((unsigned char)s1[i] - (unsigned char)s2[i]));
}

char *ft_strcat(char *dst, const char *src)
{
	char *str, *tmp;

	str = malloc(sizeof(char) * (ft_strlen(dst) + ft_strlen(src) + 1));
	if (str == NULL)
		return (NULL);

	tmp = str;
	while (*dst)
		*tmp++ = *dst++;
	while (*src)
		*tmp++ = *src++;
	*tmp = '\0';

	return (str);
}

/*
 * Searches for a string in a file.
 * Returns 1 if the string is found, 0 if not, -1 on error.d
*/
int ft_strnf(int fd, const char *str)
{
	struct stat s;
	char *fcontent;

	if (fstat(fd, &s) < 0)
		return (-1);

	// Reads the file pointed by @fd in fcontent
	fcontent = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (fcontent == NULL)
		return (-1);

	for (long int i = 0; i < s.st_size; i++)
	{
		if (ft_strncmp(str, &fcontent[i], ft_strlen(str)) == 0)
		{
			munmap(fcontent, s.st_size);
			return (1);
		}
	}
	munmap(fcontent, s.st_size);
	return (0);
}

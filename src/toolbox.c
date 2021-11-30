#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
 * Concat two strings in a newly allocated buffer.
 * Returns the new string on success, NULL on error.
*/
char *ft_strcat(char *dst, const char *src)
{
	char *str, *tmp;

	str = malloc(sizeof(char) * (strlen(dst) + strlen(src) + 1));
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
		if (strncmp(str, &fcontent[i], strlen(str)) == 0)
		{
			munmap(fcontent, s.st_size);
			return (1);
		}
	}
	munmap(fcontent, s.st_size);
	return (0);
}

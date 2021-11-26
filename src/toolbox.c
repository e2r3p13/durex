#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

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

int ft_strnf(int fd, const char *s)
{
	(void)fd;
	(void)s;
	
	return (0);
}

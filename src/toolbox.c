#include <stdio.h>
#include <stdarg.h>

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

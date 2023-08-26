#include "main.h"

/**
 * willy_sprintf - Writes a formatted string to an array of chars
 * @str: The char array to write the formatted string to
 * @fmt: The format string to use for writing
 * @...: Optional arguments used in the format string
 */
void willy_sprintf(char *str, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	willy_vsprintf(str, fmt, ap);
	va_end(ap);
}

/**
 * willy_fprintf - Prints a formatted string to a file descriptor
 * @fd: The file descriptor to write to
 * @fmt: The format string
 * @...: Optional arguments used in the format string
 */
void willy_fprintf(int fd, const char *fmt, ...)
{
	char buf[BUFFER_SIZE];
	va_list ap;

	va_start(ap, fmt);
	willy_vsprintf(buf, fmt, ap);
	willy_write(fd, buf, willy_strlen(buf));
	va_end(ap);
}

/**
 * willy_printf - Prints a formatted string to the standard output
 * @fmt: Format string containing text and format specifiers
 * @...: Optional arguments used in the format string
 */
void willy_printf(const char *fmt, ...)
{
	va_list ap;
	/* TODO: Allocate buf dynamically */
	char buf[BUFFER_SIZE];

	va_start(ap, fmt);
	willy_vsprintf(buf, fmt, ap);
	willy_write(1, buf, willy_strlen(buf));
	va_end(ap);
}

/**
 * willy_vsprintf - Writes a formatted string to a buffer
 * @str: The buffer to write to
 * @fmt: The format string
 * @ap: The list of arguments
 */
void willy_vsprintf(char *str, const char *fmt, va_list ap)
{
	char c;
	int i, state = 0;

	for (i = 0; fmt[i]; i++)
	{
		c = fmt[i];
		if (state == 0)
			(c == '%') ? (state = '%') : (*str++ = c);
		else if (state == '%')
		{
			if (c == 'd')
				str += willy_itoa(va_arg(ap, int), str, 10, 1);
			else if (c == 'l')
				str += willy_itoa(va_arg(ap, size_t), str, 10, 0);
			else if (c == 'x')
				str += willy_itoa(va_arg(ap, int), str, 16, 0);
			else if (c == 'p')
			{
				(*str++ = '0', *str++ = 'x');
				str += willy_itoa(va_arg(ap, size_t), str, 16, 0);
			}
			else if (c == 's')
				str += willy_stoa(va_arg(ap, char *), str);
			else if (c == 'c')
				*str++ = va_arg(ap, int);
			else if (c == '%')
				*str++ = c;
			else
				(*str++ = '%', *str++ = c);
			state = 0;
		}
	}
	*str = '\0';
}

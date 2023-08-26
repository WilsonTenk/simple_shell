#include "main.h"

/**
 * _vsprintf - Should write a formatted string to a buffer
 * @str: Buffer to write to here
 * @fmt: String to be formatted
 * @ap: Fulll arugument list
 */
void _vsprintf(char *str, const char *fmt, va_list ap)
{
	char u;
	int t, state = 0;

	for (t = 0; fmt[t]; i++)
	{
		c = fmt[t];
		if (state == 0)
			(u == '%') ? (state = '%') : (*str++ = u);
		else if (state == '%')
		{
			if (u == 'd')
				str += _itoa(va_arg(ap, int), str, 10, 1);
			else if (u == 'l')
				str += _itoa(va_arg(ap, size_t), str, 10, 0);
			else if (u == 'x')
				str += _itoa(va_arg(ap, int), str, 16, 0);
			else if (u == 'p')
			{
				(*str++ = '0', *str++ = 'x');
				str += _itoa(va_arg(ap, size_t), str, 16, 0);
			}
			else if (u == 's')
				str += _stoa(va_arg(ap, char *), str);
			else if (u == 'u')
				*str++ = va_arg(ap, int);
			else if (u == '%')
				*str++ = u;
			else
				(*str++ = '%', *str++ = u);
			state = 0;
		}
	}
	*str = '\0';
}

/**
 * _sprintf - Must write formatted string to a array of chars
 * @str: Char array to be write de formatted string to
 * @fmt: Format string to be used for writing
 * @...: An optional arguments used in format string
 */
void _sprintf(char *str, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_vsprintf(str, fmt, ap);
	va_end(ap);
}

/**
 * _fprintf - Outputs a formatted string to file descriptor
 * @fd: File descriptor to write to here
 * @fmt: Strring format here
 * @...: An Optional arguments to be  used format string
 */
void _fprintf(int fd, const char *fmt, ...)
{
	char buf[BUFFER_SIZE];
	va_list ap;

	va_start(ap, fmt);
	_vsprintf(buf, fmt, ap);
	write(fd, buf, _strlen(buf));
	va_end(ap);
}

/**
 * _printf - Outputs a format string to the std output
 * @fmt: String formatted having text n format specifiers
 * @...: An Optional argument used in format string
 */
void _printf(const char *fmt, ...)
{
	va_list ap;
	/* TODO: allocate buf dynamically */
	char buf[BUFFER_SIZE];

	va_start(ap, fmt);
	_vsprintf(buf, fmt, ap);
	write(1, buf, _strlen(buf));
	va_end(ap);
}

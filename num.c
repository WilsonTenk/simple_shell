#include "main.h"

/**
 * willy_is_num - Checks if a string represents a numeric value.
 * @str: The input string to be checked.
 *
 * Return: 1 if the string represents a numeric value, 0 otherwise.
 */
int willy_is_num(char *str)
{
	int i = 0;

	if (str[0] == '-' || str[0] == '+')
		i++;

	for (; str[i]; i++)
		if (str[i] >= '0' && str[i] <= '9')
			return (1);

	return (0);
}

/**
 * willy_atoi - convert a string of digits into an integer
 * @str: the string to be converted
 * Return: the converted integer
 */
int willy_atoi(const char *str)
{
	unsigned int i = 0, result = 0, sign = 1;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;

	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}

	for (; str[i] >= '0' && str[i] <= '9'; i++)
		result = result * 10 + (str[i] - '0');

	return (sign * result);
}

/**
 * willy_itoa - convert a integer to a string in the given base (n)
 * @x: the integer to convert
 * @s: the output string
 * @base: the base of the number
 * @sign: whether or not to include a sign character
 * Return: the length of the string.
 */
int willy_itoa(long x, char s[], int base, int sign)
{
	unsigned int n, i = 0, neg = 0;

	if (sign && x < 0)
	{
		neg = 1;
		n = -x;
	}
	else
		n = x;

	do
		s[i++] = n % base + '0';
	while ((n /= base) > 0);

	if (neg)
		s[i++] = '-';

	s[i] = '\0';
	willy_reverse_str(s);
	return (i);
}

/**
 * willy_realloc - reallocate a block of memory
 * @ptr: pointer to the memory previously allocated
 * @old_size: size of the memory block pointed by ptr
 * @new_size: new size of the memory block to be allocated
 * Return: pointer to the newly allocated memory, or NULL if it fails
 */
void *willy_realloc(void *ptr, ssize_t old_size, ssize_t new_size)
{
	void *new_ptr;

	if (!ptr)
		return (willy_malloc(new_size));

	if (new_size == 0)
	{
		willy_free(ptr);
		return (NULL);
	}

	new_ptr = willy_malloc(new_size);
	if (!new_ptr)
		return (NULL);

	willy_memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
	willy_free(ptr);

	return (new_ptr);
}

/**
 * willy_free_double - Frees a double pointer
 * @ptr: The double pointer to be freed
 */
void willy_free_double(char ***ptr)
{
	int i;

	for (i = 0; (*ptr)[i]; i++)
		willy_free((*ptr)[i]);
	willy_free(*ptr);
	*ptr = NULL;
}

#include "main.h"

/**
 * num_val - Lookup if string show numeric value
 * @str: We input string that is to be checked
 *
 * Return: 1 strings shows a numeric value, 0 otherwise.
 */
int num_val(char *str)
{
	int t = 0;

	if (str[0] == '-' || str[0] == '+')
		t++;

	for (; str[t]; t++)
		if (str[t] >= '0' && str[t] <= '9')
			return (1);

	return (0);
}

/**
 * _atoi - Change string of digits into integer
 * @str: String to be changed
 * Return: Changed integer
 */
int _atoi(const char *str)
{
	unsigned int t = 0, result = 0, sign = 1;

	while (str[t] == ' ' || str[t] == '\t' || str[t] == '\n')
		t++;

	if (str[t] == '-' || str[t] == '+')
	{
		if (str[t] == '-')
			sign = -1;
		t++;
	}

	for (; str[t] >= '0' && str[t] <= '9'; t++)
		result = result * 10 + (str[t] - '0');

	return (sign * result);
}

/**
 * _itoa - To change integer to string in a given base
 * @x: Interger to be changed
 * @s: String to be output
 * @base: Our number base
 * @sign: If or not to add a sign char
 * Return: String length
 */
int _itoa(long x, char s[], int base, int sign)
{
	unsigned int u, t = 0, neg = 0;

	if (sign && x < 0)
	{
		neg = 1;
		u = -x;
	}
	else
		u = x;

	do
		s[t++] = n % base + '0';
	while ((u /= base) > 0);

	if (neg)
		s[t++] = '-';

	s[t] = '\0';
	reverse_str(s);
	return (t);
}

/**
 * _realloc - Re allocate a block of mem
 * @ptr: A pointer to mem earlier allocated
 * @old_size: The size of mem block pointed by ptr
 * @new_size: Our new size of mem block to be allocated
 * Return: Pointer to new allocated mem, or NULL if fails
 */
void *_realloc(void *ptr, ssize_t old_size, ssize_t new_size)
{
	void *new_ptr;

	if (!ptr)
		return (malloc(new_size));

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);

	_memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
	free(ptr);

	return (new_ptr);
}

/**
 * free_double - This frees a double pointer
 * @ptr: Doub pointer to be freed.
 */
void free_double(char ***ptr)
{
	int t;

	for (t = 0; (*ptr)[t]; t++)
		free((*ptr)[t]);
	free(*ptr);
	*ptr = NULL;
}

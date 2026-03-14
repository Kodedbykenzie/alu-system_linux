#include "multithreading.h"



/**
* prime_factors - func
* @s: char const *
* Return: lsit_t *
*/
list_t *prime_factors(char const *s)
{
	unsigned long int n = 0, lim = 0, i = 2, *p = NULL;
	list_t *list = NULL;

	list = malloc(sizeof(list_t));
	list_init(list);
	n = strtoul(s, NULL, 10);
	if (n < 2)
	{
		p = malloc(sizeof(unsigned long int));
		*p = n;
		list_add(list, (void *)p);
	}
	else
	{
		lim = lim_binary(n);
		while (i <= lim)
		{
			if (!(n % i))
			{
				p = malloc(sizeof(unsigned long int));
				*p = i;
				list_add(list, (void *)p);
				n /= i;
				lim = lim_binary(n);
			}
			else
				i += (i == 2) ? 1 : 2;
		}
		if (n > 1)
		{
			p = malloc(sizeof(unsigned long int));
			*p = n;
			list_add(list, (void *)p);
		}
	}
	return (list);
}


/**
* lim_binary - func
* @n: unsigned long int
* Return: unsigned long int
*/
unsigned long int lim_binary(unsigned long int n)
{
	unsigned long int r = 0, end = n / 2, begin = 2, mid = (end + begin) / 2;

	while (begin < end)
	{
		r = mid * mid;
		if (r < n)
			begin = mid;
		else if (r > n)
			end = mid - 1;
		else
			break;
		mid = (end + begin) / 2 + 1;
	}
	return (mid);
}

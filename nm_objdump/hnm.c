#include "hnm.h"


/**
* main- func
* @ac: int
* @av: char **
* Return: int
*/
int main(int ac, char **av)
{
	int fd = 0, i = 1, rv = 0;

	if (ac == 1)
	{
		fd = open("a.out", O_RDONLY);
		rv = elf_handler(fd);
		if (rv == 16)
			fprintf(stderr, "%s: %s: no symbols", av[0], av[1]);
		close(fd);
	}
	else
	{
		for (; i < ac; i++)
		{
			fd = open(av[i], O_RDONLY);
			if (fd != -1)
			{
				rv = elf_handler(fd);
				if (rv == 16)
					fprintf(stderr, "%s: %s: no symbols\n", av[0], av[i]);
				close(fd);
			}
		}
	}
	return (EXIT_SUCCESS);
}


/**
* elf_handler- func
* @fd: int
* Return: int
*/
int elf_handler(int fd)
{
	char head[16];
	int rv = 0;

	if (read(fd, head, (sizeof(char) * 16)) != 16)
		return (1);
	if ((head[0] != 0x7F) || (head[1] != 'E') ||
		(head[2] != 'L') || (head[3] != 'F'))
		return (1);
	if (head[4] == ELFCLASS32)
		rv = manage_32(fd);
	if (head[4] == ELFCLASS64)
		rv = manage_64(fd);
	return (rv);
}

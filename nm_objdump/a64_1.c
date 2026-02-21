#include "hnm.h"



/**
* abs_64 - func
* @sym: symbol
* Return: char
*/
char abs_64(Elf64_Sym *sym)
{
	char c = 0;

	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
		c = ((sym->st_other == STV_DEFAULT) ||
			(sym->st_other == STV_PROTECTED)) ? 'A' : 'a';
	else
		c = (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) ? 'A' : 'a';
	return (c);
}


/**
* func_64 - func
* @sym: symbol
* Return: char
*/
char func_64(Elf64_Sym *sym)
{
	char c = 0;

	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
		c = ((sym->st_other == STV_DEFAULT) ||
			(sym->st_other == STV_PROTECTED)) ? 'W' : 'w';
	else
		c = (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) ? 'T' : 't';
	return (c);
}


/**
* stt_common_64 - func
* @sym: symbol
* Return: char
*/
char stt_common_64(Elf64_Sym *sym)
{
	char c = 0;

	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
		c = ((sym->st_other == STV_DEFAULT) ||
			(sym->st_other == STV_PROTECTED)) ? 'C' : 'c';
	else
		c = (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) ? 'C' : 'c';
	return (c);
}


/**
* p_64 - func
* @fd: int
* @d: data64_t
* @sym: symbol
* @c: char
* @st_shndx: uint16_t
* Return: int
*/
int p_64(int fd, data64_t *d, Elf64_Sym *sym, char c, uint16_t st_shndx)
{
	char		name_buf[1024];
	uint32_t	st_name = 0, n = -1;
	uint64_t	sh_strtab_off = 0, strtab_off = 0, st_value = 0;
	Elf64_Shdr	sh_strtab;

	memset(name_buf, 0, 1024);
	st_value = (d->ends) ? bswap_64(sym->st_value) : sym->st_value;
	st_name = (d->ends) ? bswap_32(sym->st_name) : sym->st_name;
	sh_strtab_off = d->e_shoff + (d->sh_link * sizeof(Elf64_Shdr));
	lseek(fd, sh_strtab_off, SEEK_SET);
	if (read(fd, &sh_strtab, sizeof(Elf64_Shdr)) != sizeof(Elf64_Shdr))
		return (1);
	strtab_off = (d->ends) ? bswap_64(sh_strtab.sh_offset) : sh_strtab.sh_offset;
	lseek(fd, (strtab_off + st_name), SEEK_SET);
	do {
		n++;
		if (read(fd, name_buf + n, 1) != 1)
			return (1);
	} while (name_buf[n]);
	if (st_shndx == SHN_UNDEF)
		printf("                 %c %s\n", c, name_buf);
	else
		printf("%016lx %c %s\n", st_value, c, name_buf);
	return (0);
}

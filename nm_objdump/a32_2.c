#include "hnm.h"



/**
* mon_32 - func
* @fd: int
* @d: data32_t
* @sym: symbol
* @st_shndx: uint16_t
* Return: char
*/
char mon_32(int fd, data32_t *d, Elf32_Sym *sym, uint16_t st_shndx)
{
	char c = 0, buffer[1024];

	if (buffer_read_32(fd, d, st_shndx, buffer))
		return (-1);
	if (ELF32_ST_TYPE(sym->st_info) == STT_OBJECT)
		c = object_32(sym, buffer);
	else /* if (ELF32_ST_TYPE(sym->st_info) == STT_NOTYPE) */
		c = notype_32(sym, buffer);
	return (c);
}


/**
* buffer_read_32 - func
* @fd: int
* @d: data32_t
* @st_shndx: uint16_t
* @buffer: char *
* Return: int
*/
int buffer_read_32(int fd, data32_t *d, uint16_t st_shndx, char *buffer)
{
	uint32_t	offset = 0, sh_name = 0, sto = 0, pos = -1;
	Elf32_Shdr	strtab, sobj;

	memset(buffer, 0, 1024);
	offset = d->e_shoff + st_shndx * sizeof(Elf32_Shdr);
	lseek(fd, offset, SEEK_SET);
	if (read(fd, &sobj, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
		return (1);
	offset = d->e_shoff + d->e_shstrndx * sizeof(Elf32_Shdr);
	lseek(fd, offset, SEEK_SET);
	if (read(fd, &strtab, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
		return (1);
	sto = (d->ends) ? bswap_32(strtab.sh_offset) : strtab.sh_offset;
	sh_name = (d->ends) ? bswap_32(sobj.sh_name) : sobj.sh_name;
	lseek(fd, sto + sh_name, SEEK_SET);
	do {
		pos++;
		if (read(fd, buffer + pos, 1) != 1)
			return (1);
	} while (buffer[pos] && (pos < 1024));
	return (0);
}


/**
* object_32 - func
* @sym: symbol
* @buffer: char *
* Return: char
*/
char object_32(Elf32_Sym *sym, char *buffer)
{
	char c = 0;

	if (ELF32_ST_BIND(sym->st_info) == STB_WEAK)
		c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'V' : 'v';
	else if (!strcmp(buffer, ".bss"))
		c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 'b' : 'B';
	else if (!strcmp(buffer, ".rodata") || !strcmp(buffer, ".interp") ||
				!strcmp(buffer, ".init") || !strcmp(buffer, ".fini"))
		c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 'r' : 'R';
	else if (!strcmp(buffer, ".text") || !strcmp(buffer, ".plt") ||
				!strcmp(buffer, ".text.startup"))
		c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 't' : 'T';
	else
		c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 'd' : 'D';
	return (c);
}


/**
* notype_32 - func
* @sym: symbol
* @buffer: char *
* Return: char
*/
char notype_32(Elf32_Sym *sym, char *buffer)
{
	char c = 0;

	if (!strcmp(buffer, ".text") || !strcmp(buffer, ".plt"))
	{
		if (ELF32_ST_BIND(sym->st_info) == STB_WEAK)
			c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'W' : 'w';
		else
			c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 't' : 'T';
	}
	else if (!strcmp(buffer, ".rodata") || !strcmp(buffer, ".interp") ||
				!strcmp(buffer, ".init") || !strcmp(buffer, ".fini"))
	{
		if (ELF32_ST_BIND(sym->st_info) == STB_WEAK)
			c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'V' : 'v';
		else
			c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 'r' : 'R';
	}
	else if (!strcmp(buffer, ".data") || !strcmp(buffer, ".jcr") ||
			!strcmp(buffer, ".ctors") || !strcmp(buffer, ".dtors") ||
			!strcmp(buffer, ".got"))
	{
		if (ELF32_ST_BIND(sym->st_info) == STB_WEAK)
			c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'V' : 'v';
		else
			c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 'd' : 'D';
	}
	else
	{
		if (ELF32_ST_BIND(sym->st_info) == STB_WEAK)
			c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'W' : 'w';
		else
			c = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) ? 'n' : 'N';
	}
	return (c);
}




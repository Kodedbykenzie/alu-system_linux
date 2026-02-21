#include "hnm.h"



/**
* mon_64 - func
* @fd: int
* @d: data64_t
* @sym: symbol
* @st_shndx: uint64_t
* Return: char
*/
char mon_64(int fd, data64_t *d, Elf64_Sym *sym, uint32_t st_shndx)
{
	char c = 0, buffer[1024];

	if (buffer_read_64(fd, d, st_shndx, buffer))
		return (-1);
	if (ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
		c = object_64(sym, buffer);
	else /* if (ELF64_ST_TYPE(sym->st_info) == STT_NOTYPE) */
		c = notype_64(sym, buffer);
	return (c);
}


/**
* buffer_read_64 - func
* @fd: int
* @d: data64_t
* @st_shndx: uint16_t
* @buffer: char *
* Return: int
*/
int buffer_read_64(int fd, data64_t *d, uint16_t st_shndx, char *buffer)
{
	uint32_t	sh_name = 0, sto = 0, pos = -1;
	uint64_t	offset = 0;
	Elf64_Shdr	sobj, strtab;

	memset(buffer, 0, 1024);
	offset = d->e_shoff + st_shndx * sizeof(Elf64_Shdr);
	lseek(fd, offset, SEEK_SET);
	if (read(fd, &sobj, sizeof(Elf64_Shdr)) != sizeof(Elf64_Shdr))
		return (1);
	offset = d->e_shoff + d->e_shstrndx * sizeof(Elf64_Shdr);
	lseek(fd, offset, SEEK_SET);
	if (read(fd, &strtab, sizeof(Elf64_Shdr)) != sizeof(Elf64_Shdr))
		return (1);
	sto = (d->ends) ? bswap_64(strtab.sh_offset) : strtab.sh_offset;
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
* object_64 - func
* @sym: symbol
* @buffer: char *
* Return: char
*/
char object_64(Elf64_Sym *sym, char *buffer)
{
	char c = 0;

	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
		c = ((sym->st_other == STV_DEFAULT) ||
			(sym->st_other == STV_PROTECTED)) ? 'V' : 'v';
	else if (!strcmp(buffer, ".bss") || !strcmp(buffer, "completed.7585"))
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'b' : 'B';
	else if (!strcmp(buffer, ".rodata") || !strcmp(buffer, ".interp") ||
		!strcmp(buffer, ".init") || !strcmp(buffer, ".fini") ||
		!strcmp(buffer, ".eh_frame"))
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'r' : 'R';
	else if (!strcmp(buffer, ".text") || !strcmp(buffer, ".plt") ||
		!strcmp(buffer, ".text.startup") || !strcmp(buffer, ".fini_array") ||
		!strcmp(buffer, ".init_array"))
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 't' : 'T';
	else
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'd' : 'D';
	return (c);
}


/**
* notype_64 - func
* @sym: symbol
* @buffer: char *
* Return: char
*/
char notype_64(Elf64_Sym *sym, char *buffer)
{
	char c = 0;

	if (!strcmp(buffer, ".fini_array") || !strcmp(buffer, ".init_array"))
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 't' : 'T';
	else if (!strcmp(buffer, ".bss") || !strcmp(buffer, "completed.7585"))
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'b' : 'B';
	else if (!strcmp(buffer, ".text") || !strcmp(buffer, ".plt"))
	{
		if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
			c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'W' : 'w';
		else
			c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 't' : 'T';
	}
	else if (!strcmp(buffer, ".rodata") || !strcmp(buffer, ".interp") ||
		!strcmp(buffer, ".init") || !strcmp(buffer, ".fini") ||
		!strcmp(buffer, ".eh_frame_hdr"))
	{
		if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
			c = ((sym->st_other == STV_DEFAULT) ||
				(sym->st_other == STV_PROTECTED)) ? 'V' : 'v';
		else
			c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'r' : 'R';
	}
	else if (!strcmp(buffer, ".data") || !strcmp(buffer, ".jcr") ||
			!strcmp(buffer, ".ctors") || !strcmp(buffer, ".dtors") ||
			!strcmp(buffer, ".got"))
	{
		c = (ELF64_ST_BIND(sym->st_info) == STB_WEAK) ?
		(((sym->st_other == STV_DEFAULT) || (sym->st_other == STV_PROTECTED)) ?
		'W' : 'w') : ((ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'd' : 'D');
	}
	else
	{
		c = (ELF64_ST_BIND(sym->st_info) == STB_WEAK) ?
		(((sym->st_other == STV_DEFAULT) || (sym->st_other == STV_PROTECTED)) ?
		'W' : 'w') : ((ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'n' : 'N');
	}
	return (c);
}

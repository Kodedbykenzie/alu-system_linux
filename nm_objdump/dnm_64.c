#include "hnm.h"

#define SS64 sizeof(Elf64_Sym)

/**
* manage_64 - func
* @fd: int
* Return: 0 on Success, 1 on Failure
*/
int manage_64(int fd)
{
	data64_t	d;
	Elf64_Shdr	sh;
	uint16_t	i = 0, symflag = 16;
	uint32_t	sh_type = 0;
	uint64_t	shdr_pos = 0, sh_size = 0, sh_offset = 0;

	lseek(fd, 0, SEEK_SET);
	if (set_d64(fd, &d))
		return (1);
	if (d.e_shstrndx == SHN_XINDEX)
	{
		lseek(fd, d.e_shoff, SEEK_SET);
		if (read(fd, &sh, sizeof(Elf64_Shdr)) != sizeof(Elf64_Shdr))
			return (1);
		d.e_shstrndx = (d.ends) ? bswap_64(sh.sh_link) : sh.sh_link;
	}
	shdr_pos = lseek(fd, d.e_shoff, SEEK_SET);
	for (; i < d.e_shnum; i++)
	{
		lseek(fd, shdr_pos, SEEK_SET);
		if (read(fd, &sh, sizeof(Elf64_Shdr)) != sizeof(Elf64_Shdr))
			return (1);
		shdr_pos += sizeof(Elf64_Shdr);
		sh_size = ((d.ends) ? bswap_64(sh.sh_size) : sh.sh_size) / SS64;
		if (!sh_size)
			continue;
		sh_type = (d.ends) ? bswap_32(sh.sh_type) : sh.sh_type;
		d.sh_link = (d.ends) ? bswap_64(sh.sh_link) : sh.sh_link;
		if (sh_type == SHT_SYMTAB)
		{
			symflag = 0;
			sh_offset = (d.ends) ? bswap_64(sh.sh_offset) : sh.sh_offset;
			lseek(fd, sh_offset, SEEK_SET);
			if (manage_sym64_list(fd, &d, sh_size))
				return (1);
		}
	}
	return (symflag);
}


/**
* set_d64 - func
* @fd: int
* @d: data64_t
* Return: int
*/
int set_d64(int fd, data64_t *d)
{
	Elf64_Ehdr	h;

	if (read(fd, &h, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr))
		return (1);
	d->ends = h.e_ident[EI_DATA] == ELFDATA2MSB;
	d->e_shnum = (d->ends) ? bswap_16(h.e_shnum) : h.e_shnum;
	d->e_shoff = (d->ends) ? bswap_64(h.e_shoff) : h.e_shoff;
	d->e_shstrndx = (d->ends) ? bswap_16(h.e_shstrndx) : h.e_shstrndx;
	return (0);
}


/**
* manage_sym64_list - func
* @fd: int
* @d: data64_t
* @size: amount of sym structs to read
* Return: 0 on Success, 1 on Failure
*/
int manage_sym64_list(int fd, data64_t *d, uint64_t size)
{
	uint64_t j = 0, pos = 0;
	Elf64_Sym sym;

	pos = lseek(fd, 0, SEEK_CUR);
	for (; j < size; j++)
	{
		lseek(fd, pos, SEEK_SET);
		if (read(fd, &sym, sizeof(Elf64_Sym)) != sizeof(Elf64_Sym))
			return (1);
		pos += sizeof(Elf64_Sym);
		if (!sym.st_name)
			continue;
		m64(fd, d, &sym);
	}
	return (0);
}


/**
* m64 - func
* @fd: int
* @d: data64_t
* @sym: symbol
* Return: int
*/
int m64(int fd, data64_t *d, Elf64_Sym *sym)
{
	char		c = 0;
	uint16_t	st_shndx = 0;

	st_shndx = (d->ends) ? bswap_16(sym->st_shndx) : sym->st_shndx;
	if (st_shndx == SHN_UNDEF)
		c = (ELF64_ST_BIND(sym->st_info) == STB_WEAK) ? 'w' : 'U';
	else if (ELF64_ST_TYPE(sym->st_info) == STT_FILE)
	{
		if (!sym->st_value)
			return (1);
		c = (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) ? 'F' : 'f';
	}
	else if (st_shndx == SHN_ABS)
		c = abs_64(sym);
	else if (ELF64_ST_TYPE(sym->st_info) == STT_FUNC)
		c = func_64(sym);
	else if (st_shndx == SHN_COMMON)
		c = (ELF64_ST_BIND(sym->st_info) == STB_WEAK) ? 'c' : 'C';
	else if ((ELF64_ST_TYPE(sym->st_info) == STT_OBJECT) ||
			(ELF64_ST_TYPE(sym->st_info) == STT_NOTYPE))
	{
		c = mon_64(fd, d, sym, st_shndx);
		if (c == -1)
			return (1);
	}
	else if (ELF64_ST_TYPE(sym->st_info) == STT_COMMON)
		c = stt_common_64(sym);
	else if (ELF64_ST_TYPE(sym->st_info) == STT_SECTION)
		c = (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) ? 'S' : 's';
	else if (ELF64_ST_TYPE(sym->st_info) == STT_LOPROC ||
			ELF64_ST_TYPE(sym->st_info) == STT_HIPROC)
		c = 'p';
	else
		return (1);
	if (p_64(fd, d, sym, c, st_shndx))
		return (1);
	return (0);
}

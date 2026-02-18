#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>
#include "elf_utils.h"

/**
 * get_symbol_type - returns the nm type character of a symbol
 * @sym: pointer to symbol
 * @sections: pointer to section headers
 *
 * Return: symbol type character
 */
static char get_symbol_type(Elf64_Sym *sym, Elf64_Shdr *sections)
{
	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
	{
		if (sym->st_shndx == SHN_UNDEF)
			return ('w');
		return ('W');
	}
	if (sym->st_shndx == SHN_UNDEF)
		return ('U');
	if (sym->st_shndx == SHN_ABS)
		return ('A');
	if (sections[sym->st_shndx].sh_type == SHT_NOBITS)
		return ('B');
	if (sections[sym->st_shndx].sh_flags & SHF_EXECINSTR)
		return ('T');
	if ((sections[sym->st_shndx].sh_flags & SHF_ALLOC) &&
	    (sections[sym->st_shndx].sh_flags & SHF_WRITE))
		return ('D');
	if (sections[sym->st_shndx].sh_flags & SHF_ALLOC)
		return ('R');
	return ('?');
}

/**
 * print_symbols - prints symbols from symtab
 * @data: mapped file
 * @shdr: section headers
 * @symtab_hdr: symbol table section header
 *
 * Return: void
 */
static void print_symbols(void *data, Elf64_Shdr *shdr,
			  Elf64_Shdr *symtab_hdr)
{
	Elf64_Sym *symtab;
	Elf64_Shdr *strtab_hdr;
	char *strtab;
	int symcount;
	int j;
	char type;

	symtab = (Elf64_Sym *)((char *)data + symtab_hdr->sh_offset);
	symcount = symtab_hdr->sh_size / sizeof(Elf64_Sym);
	strtab_hdr = &shdr[symtab_hdr->sh_link];
	strtab = (char *)data + strtab_hdr->sh_offset;

	for (j = 0; j < symcount; j++)
	{
		if (symtab[j].st_name == 0)
			continue;
		type = get_symbol_type(&symtab[j], shdr);
		if (type == 'U')
			printf("                 %c %s\n",
			       type, strtab + symtab[j].st_name);
		else
			printf("%016lx %c %s\n",
			       symtab[j].st_value,
			       type,
			       strtab + symtab[j].st_name);
	}
}

/**
 * process_file - processes an ELF file and prints symbols
 * @filename: file to process
 *
 * Return: void
 */
void process_file(char *filename)
{
	int fd;
	struct stat st;
	void *data;
	Elf64_Ehdr *ehdr;
	Elf64_Shdr *shdr;
	int i;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return;
	if (fstat(fd, &st) < 0)
	{
		close(fd);
		return;
	}
	data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == MAP_FAILED)
	{
		close(fd);
		return;
	}
	ehdr = (Elf64_Ehdr *)data;
	shdr = (Elf64_Shdr *)((char *)data + ehdr->e_shoff);

	for (i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
			print_symbols(data, shdr, &shdr[i]);
	}
	munmap(data, st.st_size);
	close(fd);
}

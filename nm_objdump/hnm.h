#ifndef HNM_H
#define HNM_H



/* Libraries */
	#include <byteswap.h>

	#include <elf.h>

	#include <fcntl.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include <sys/stat.h>

	#include <unistd.h>


/**
 * struct data32_s - structure
 * @ends: uint8_t
 * @e_shnum: uint16_t
 * @e_shstrndx: uint16_t
 * @e_shoff: uint32_t
 * @sh_link: uint32_t
 */
typedef struct data32_s
{
	uint8_t		ends;
	uint16_t	e_shnum;
	uint32_t	e_shoff,
				e_shstrndx,
				sh_link;
} data32_t;


/**
 * struct data64_s - structure
 * @ends: uint8_t
 * @e_shnum: uint16_t
 * @e_shstrndx: uint16_t
 * @e_shoff: uint32_t
 * @sh_link: uint32_t
 */
typedef struct data64_s
{
	uint8_t		ends;
	uint16_t	e_shnum;
	uint64_t	e_shoff,
				e_shstrndx,
				sh_link;
} data64_t;



/* Functions */

/* hnm.c */
int main(int ac, char **av);
int elf_handler(int fd);


/* dnm_32.c */
int manage_32(int fd);
int set_d32(int fd, data32_t *d);
int manage_sym32_list(int fd, data32_t *d, uint32_t size);
int m32(int fd, data32_t *d, Elf32_Sym *sym);


/* a32_1.c */
char abs_32(Elf32_Sym *sym);
char func_32(Elf32_Sym *sym);
char stt_common_32(Elf32_Sym *sym);
int p_32(int fd, data32_t *d, Elf32_Sym *sym, char c, uint16_t st_shndx);


/* a32_2.c */
char mon_32(int fd, data32_t *d, Elf32_Sym *sym, uint16_t st_shndx);
int buffer_read_32(int fd, data32_t *d, uint16_t st_shndx, char *buffer);
char object_32(Elf32_Sym *sym, char *buffer);
char notype_32(Elf32_Sym *sym, char *buffer);


/* dnm_64.c */
int manage_64(int fd);
int set_d64(int fd, data64_t *d);
int manage_sym64_list(int fd, data64_t *d, uint64_t size);
int m64(int fd, data64_t *d, Elf64_Sym *sym);


/* a64_1.c */
char abs_64(Elf64_Sym *sym);
char func_64(Elf64_Sym *sym);
char stt_common_64(Elf64_Sym *sym);
int p_64(int fd, data64_t *d, Elf64_Sym *sym, char c, uint16_t st_shndx);


/* a64_2.c */
char mon_64(int fd, data64_t *d, Elf64_Sym *sym, uint32_t st_shndx);
int buffer_read_64(int fd, data64_t *d, uint16_t st_shndx, char *buffer);
char object_64(Elf64_Sym *sym, char *buffer);
char notype_64(Elf64_Sym *sym, char *buffer);



#endif

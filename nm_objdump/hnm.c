nclude <stdio.h>
#include <stdlib.h>
#include "elf_utils.h"

int main(int argc, char **argv)
{
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <objfile ...>\n", argv[0]);
        return (1);
    }

    for (i = 1; i < argc; i++)
        process_file(argv[i]);

    return (0);
}


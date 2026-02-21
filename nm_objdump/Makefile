.PHONY: all clean fclean re hnm hobjdump

CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -pedantic

# Sources for hnm
NM_SRC = hnm.c dnm_32.c dnm_64.c a32_1.c a32_2.c a64_1.c a64_2.c
NM_NAME = hnm

# Sources for hobjdump
HOBJ_SRC = hobjdump.c dnm_32.c dnm_64.c a32_1.c a32_2.c a64_1.c a64_2.c
HOBJ_NAME = hobjdump

all: hnm hobjdump

# Build hnm
hnm:
	$(CC) $(CFLAGS) $(NM_SRC) -o $(NM_NAME)

# Build hobjdump
hobjdump:
	$(CC) $(CFLAGS) $(HOBJ_SRC) -o $(HOBJ_NAME)

clean:
	rm -f $(NM_NAME) $(HOBJ_NAME)

fclean: clean

re: fclean all

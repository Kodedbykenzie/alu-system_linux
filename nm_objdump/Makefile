CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = hnm.c elf_utils.c
OBJ = $(SRC:.c=.o)

NAME = hnm

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

NAME = ft_ping
INCLUDE = -Iinclude

SRC_DIR = ./srcs
SRCS = $(wildcard $(SRC_DIR)/*.c)
#SRCS = $(addprefix $(SRC_DIR)/, *.c)
OBJS = $(SRCS:.c=.o)

# COLORS
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

CFLAGS = -Wall -Werror -Wextra -pedantic
ifdef DEBUG
 CFLAGS += -g -fsanitize=address
else
 CFLAGS += -Ofast
endif
ifdef OPCOUNT
 CFLAGS += -D OPCOUNT
endif
SHELL := /bin/bash
export SHELL
export DEBUG

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@
	@printf "$(PINK)Done building push_swap $(RESET)\n"

%.o: %.c
#	echo -e "$(BLUE) Compiling file $< to $@ $(RESET)"
	$(CC) -c $(CFLAGS) $(INCLUDE) $^ -o $@

clean:
	/bin/rm -f $(OBJS)
	@/bin/rm -f *.o *~ *.gch

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

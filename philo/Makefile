NAME := philo
SRCFILES := philo.c inits.c utils.c cleanup.c routine.c status.c
OBJS := $(SRCFILES:.c=.o)
INCLUDES := philo.h
CC := gcc
CFLAGS := -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES)
	@echo "Building $(NAME)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Removing object files"
	@rm -f $(OBJS)

fclean: clean
	@echo "Removing object files and $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

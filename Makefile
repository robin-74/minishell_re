CC      = cc
CFLAGS  = -Wall -Wextra -Werror

SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I headers -I libft -I ft_printf

LIBFT   = libft/libft.a
FTPRINTF = ft_printf/ft_printf.a
STACK_DIR = stack
STACK_LIB = $(STACK_DIR)/libstack.a

SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME    = minishell

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(FTPRINTF) $(STACK_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(FTPRINTF) $(STACK_LIB) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C libft

$(FTPRINTF):
	make -C ft_printf

$(STACK_LIB):
	make -C $(STACK_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C libft clean
	make -C ft_printf clean
	make -C $(STACK_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean
	make -C ft_printf fclean
	make -C $(STACK_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

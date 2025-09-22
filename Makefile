# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/29 03:37:58 by yzhang2           #+#    #+#              #
#    Updated: 2025/09/22 11:16:13 by yzhang2          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = pipex
BONUS_NAME  = pipex_bonus

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -fPIE
INCLUDES    = -I includes -I libft

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRCS        = $(wildcard src/*.c)
OBJS        = $(SRCS:.c=.o)

BONUS_SRCS  = $(wildcard bonus/*.c)
BONUS_OBJS  = $(BONUS_SRCS:.c=.o)

.PHONY: all bonus clean fclean re

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)"

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBFT)

bonus: $(LIBFT) $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BONUS_NAME) $(BONUS_OBJS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	@rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(BONUS_NAME) $(LIBFT)

re: fclean all

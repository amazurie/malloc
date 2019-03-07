# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amazurie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/12 11:58:21 by amazurie          #+#    #+#              #
#    Updated: 2019/03/07 17:14:43 by amazurie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

HOST	= libft_malloc.so
NAME	= libft_malloc_$(HOSTTYPE).so 
CC	= gcc
FLAGS	= -Wall -Wextra -Werror
DFLAGS	= -MMD
LIB	= libft/libft.a
HEADER	= includes/
H_NAME	= malloc.h
LIBSRC	= libft/
SRCDIR	= srcs/
OBJDIR	= objs/
SRC	=	malloc.c \
		area.c \
		free.c \
		realloc.c \
		block.c \
		show_alloc_mem.c \
		free_block.c \
		utils.c

# colors
GRN     =   \033[0;32m
YEL     =   \033[0;33m
CYN     =   \033[0;36m
NC      =   \033[0m
SRCS    = $(addprefix $(SRCDIR), $(SRC))
OBJS    = $(addprefix $(OBJDIR), $(SRC:.c=.o))
DEPS	:= $(OBJS:.o=.d)

all: checklib $(OBJDIR) $(NAME)

checklib:
	@make -C $(LIBSRC)

$(NAME): $(LIB) $(OBJS)
	@$(CC) $(FLAGS) -L./$(LIBSRC) -lft -ltermcap -shared -o $(NAME) $(OBJS)
	@ln -sf $(NAME) $(HOST)
	@echo "\r\c"
	@tput cd
	@echo "${GRN}$(NAME) created${NC}"
	@echo "${CYN}PROCESSING DONE !${NC}"
	@echo "${NC}"

$(OBJDIR):
	@mkdir -p objs

$(LIB):
	@echo "${CYN}Processing ${NC}./libft ${CYN}[${NC}...${CYN}]${NC}"
	@make checklib
	@echo "\n${CYN}Processing ${NC}./objs ${CYN}[${NC}...${CYN}]${NC}"

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADER)$(H_NAME)
	@echo "\r\c"
	@tput cd
	@echo "${GRN}Compiling${NC} $@\c"
	@$(CC) $(FLAGS) $(DFLAGS) -c -o $@ $< -I $(HEADER)

clean:
	@echo "${YEL}Cleaning ${NC}./objs/ ${YEL}[${NC}...${YEL}]${NC}"
	@rm -f $(OBJS)
	@rm -f $(DEPS)
	@echo "${YEL}Cleaning ${NC}./libft ${YEL}[${NC}...${YEL}]${NC}"
	@make -C $(LIBSRC) clean

fclean: clean
	@echo "${YEL}Cleaning ${NC}./${YEL}$(NAME)${NC}"
	@rm -f $(NAME)
	@rm -f $(HOST)
	@echo "${YEL}Cleaning ${NC}./libft/${YEL}libft.h${NC}\n"
	@make -C $(LIBSRC) fclean
	@rm -rf objs/
	@echo "${YEL}DELETE DONE !${NC}\n"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re

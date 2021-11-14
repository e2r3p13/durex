# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/14 17:59:02 by bccyv             #+#    #+#              #
#    Updated: 2021/11/14 19:09:44 by bccyv            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ****************
#	Variables:

NAME	=	Durex
BPATH	=	/tmp/

SRCDIR	=	src
INCDIR	=	inc
OBJDIR	=	obj

SRCS	=	main.c

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror

OBJS	=	$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DPDCS	=	$(OBJS:.o=.d)

OBJCTT	=	$(shell ls $(OBJDIR)/*.o)
DPDCTT	=	$(shell ls $(OBJDIR)/*.d)

# ****************
#	Rules:

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "[\e[32mOK\e[0m] %s\n" $@

-include $(DPDCS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -MMD -I $(INCDIR) -c $< -o $@ -DBPATH=\"$(BPATH)\" -DNAME=\"$(NAME)\"
	@printf "[\e[32mCC\e[0m] %s\n" $@

clean: _clean

fclean: _clean
ifeq ($(shell ls -1 | grep -w $(NAME)), $(NAME))
	@rm -f $(NAME)
	@printf "[\e[31mCLEAN\e[0m] %s\n" $(NAME)
endif

_clean:
ifeq ($(shell ls -1 | grep -w $(OBJDIR)), $(OBJDIR))
	@rm -rf $(OBJCTT) $(DPDCTT)
	@printf "[\e[31mCLEAN\e[0m] %s\n" $(OBJCTT)
	@rm -rf $(OBJDIR)
endif

re: fclean all

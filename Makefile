# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bccyv <bccyv@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/14 17:59:02 by bccyv             #+#    #+#              #
#    Updated: 2021/11/24 19:59:48 by bccyv            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ****************
#	Variables:

NAME	=	Durex
BPATH	=	/tmp/

SRCDIR	=	src
INCDIR	=	inc
OBJDIR	=	obj

SRCS	=	greed.c			\
			greed.map.c		\
			greed.moves.c	\
			main.c			\
			toolbox.c		\
			stub.c			\

STUB	=	durex

CC		=	clang -m64
CFLAGS	=	-Wall -Wextra -Werror #-g3 -fsanitize=address

OBJS	=	$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DPDCS	=	$(OBJS:.o=.d)

OBJCTT	=	$(shell ls $(OBJDIR)/*.o)
DPDCTT	=	$(shell ls $(OBJDIR)/*.d)

# ****************
#	Rules:

all: $(NAME)

$(NAME): $(OBJDIR)/$(STUB).o $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(DEBUG) -o $(NAME)
	@printf "[\e[32mOK\e[0m] %s\n" $@

-include $(DPDCS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -MMD -I $(INCDIR) -c $< -o $@
	@printf "[\e[32mCC\e[0m] %s\n" $@

$(SRCDIR)/stub.c: $(SRCDIR)/$(STUB).c $(SRCDIR)/ft_daemon.c
	@$(CC) $(CFLAGS) $(SRCDIR)/$(STUB).c $(SRCDIR)/ft_daemon.c -lcrypt -I$(INCDIR) -o $(STUB)
	@printf "[\e[32mCC\e[0m] %s\n" $(STUB)
	@xxd -i $(STUB) $(SRCDIR)/stub.c
	@sed -i 's/durex/stub/g' $(SRCDIR)/stub.c
	@printf "[\e[33mNEW\e[0m] %s\n" stub.c
	@rm -f $(STUB)

clean: _clean

fclean: _clean
ifeq ($(shell ls -1 | grep -w $(NAME)), $(NAME))
	@rm -f $(NAME)
	@printf "[\e[31mCLEAN\e[0m] %s\n" $(NAME)
endif
ifeq ($(shell ls -1 /bin | grep -w $(NAME)), $(NAME))
	@sh sanitize.sh
	@printf "[\e[1;95mMachine sanitized\e[0m]\n"
else ifneq ($(shell pidof $(NAME)),)
	@sh sanitize.sh
	@printf "[\e[1;95mMachine sanitized\e[0m]\n"
else ifneq ($(shell cat /etc/crontab | grep $(NAME)),)
	@sh sanitize.sh
	@printf "[\e[1;95mMachine sanitized\e[0m]\n"
else ifeq ($(shell ls -1 /run | grep -w Durex.pid), Durex.pid)
	@sh sanitize.sh
	@printf "[\e[1;95mMachine sanitized\e[0m]\n"
endif

_clean:
ifeq ($(shell ls -1 | grep -w $(OBJDIR)), $(OBJDIR))
	@rm -rf $(OBJCTT) $(DPDCTT)
	@printf "[\e[31mCLEAN\e[0m] %s\n" $(OBJCTT)
	@rm -rf $(OBJDIR)
endif
ifeq ($(shell ls -1 $(SRCDIR) | grep -w stub.c), stub.c)
	@rm -f $(SRCDIR)/stub.c
	@printf "[\e[31mCLEAN\e[0m] %s\n" stub.c
endif

re: fclean all

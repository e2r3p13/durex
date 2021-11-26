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

SRCS	=	lure.c			\
			daemonize.c		\
			toolbox.c		\
			main.c			\
			stub.c			\

STUB	=	durex

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror
DEBUG	=	-DDEBUG

ifeq ($(shell uname), Linux)
	LIBS = -lcrypt
endif

OBJS	=	$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DPDCS	=	$(OBJS:.o=.d)

OBJCTT	=	$(shell ls $(OBJDIR)/*.o)
DPDCTT	=	$(shell ls $(OBJDIR)/*.d)

# ****************
#	Rules:

all: $(NAME)

$(NAME): $(OBJDIR)/$(STUB).o $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(DEBUG) -o $(NAME)
	@printf "[\e[32mOK\e[0m] %s\n" $@

-include $(DPDCS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -MMD -I $(INCDIR) -c $< $(LIBS) $(DEBUG) -o $@ -DBPATH=\"$(BPATH)\" -DNAME=\"$(NAME)\"
	@printf "[\e[32mCC\e[0m] %s\n" $@

$(SRCDIR)/stub.c: $(SRCDIR)/$(STUB).c
	@$(CC) $(CFLAGS) $(SRCDIR)/$(STUB).c $(LIBS) -I$(INCDIR) -o $(STUB)
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

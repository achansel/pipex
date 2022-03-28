SRCS =	pipex.c
BSRCS =	pipex_bonus.c	\
		pipex2_bonus.c	\
		strncat_bonus.c	

ifndef WITH_BONUS
	OBJS = $(SRCS:.c=.o)
else
	OBJS = $(BSRCS:.c=.o)
endif

NAME = pipex

CFLAGS = -Wall -Wextra -Werror

%.o :	%.c
		gcc -o $@ -c $< $(CFLAGS)

all : $(NAME)

$(NAME) :	$(OBJS)
	gcc $(CFLAGS) -o $(NAME) $(OBJS)

bonus :
	make WITH_BONUS=1 all

clean :
	rm -f $(SRCS:.c=.o) $(BSRCS:.c=.o) 

fclean : 	clean
	rm -f $(NAME)

re : fclean all

.PHONY : all bonus clean fclean re
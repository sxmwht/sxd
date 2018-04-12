CC = gcc

SRCS = main.c gui.c

OBJS = $(SRCS:.c=.o)

PROJ_NAME = sxd

CARGS = 

LIBS = -lncurses -lm

INC = inc/

all: proj

proj: sxd


sxd: $(OBJS)
	$(CC) $(CARGS)-o $@ $^ $(LIBS)

main.o: main.c
	$(CC) -c $(CARGS) $< -o $@ -Iinc

gui.o: gui.c
	$(CC) -c $(CARGS) $< -o $@ -Iinc

clean:
	rm *.o $(PROJ_NAME)

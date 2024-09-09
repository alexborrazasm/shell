# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
main.out: main.c shell.h p0.h shell.o p0.o
	$(CC) $(CFLAGS) -o main.out main.c shell.o p0.o

shell.o: shell.h shell.c 
	$(CC) $(CFLAGS) -c shell.c

p0.o: p0.h p0.c
	$(CC) $(CFLAGS) -c p0.c

test.out: test.c shell.h shell.o
	$(CC) $(CFLAGS) -o test.out test.c shell.o

clean: 
	rm main.out shell.o p0.o test.out


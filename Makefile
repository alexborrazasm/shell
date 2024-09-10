# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
main.out: main.c types.h shell.h p0.h shell.o p0.o
	$(CC) $(CFLAGS) -o main.out main.c shell.o p0.o

shell.o: shell.h shell.c 
	$(CC) $(CFLAGS) -c shell.c

p0.o: p0.h p0.c
	$(CC) $(CFLAGS) -c p0.c

historyList: types.h historyList.h historyList.c
	$(CC) $(CFLAGS) -c historyList.c

test.out: test.c types.h shell.h historyList.h shell.o historyList.o
	$(CC) $(CFLAGS) -o test.out test.c shell.o historyList.o

clean: 
	rm main.out shell.o p0.o historyList.o test.out


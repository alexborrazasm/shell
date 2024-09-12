# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
main.out: main.c types.h shell.h historyList.h p0lib.h shell.o historyList.o p0lib.o
	$(CC) $(CFLAGS) -o main.out main.c shell.o historyList.o p0lib.o

shell.o: shell.h p0lib.h shell.c
	$(CC) $(CFLAGS) -c shell.c

p0lib.o: p0lib.h p0lib.c
	$(CC) $(CFLAGS) -c p0lib.c

historyList.o: types.h historyList.h historyList.c
	$(CC) $(CFLAGS) -c historyList.c

test.out: test.c types.h shell.h historyList.h shell.o historyList.o
	$(CC) $(CFLAGS) -o test.out test.c shell.o historyList.o

clean: 
	rm *.out *.o


# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
main.out: main.c types.h shell.h historicList.h libp0.h shell.o historicList.o libp0.o
	$(CC) $(CFLAGS) -o main.out main.c shell.o historicList.o libp0.o

shell.o: types.h shell.h libp0.h shell.c
	$(CC) $(CFLAGS) -c shell.c

libp0.o: types.h libp0.h libp0.c
	$(CC) $(CFLAGS) -c libp0.c

historicList.o: types.h historicList.h historicList.c
	$(CC) $(CFLAGS) -c historicList.c

test.out: test.c types.h shell.h historicList.h shell.o historicList.o
	$(CC) $(CFLAGS) -o test.out test.c shell.o historicList.o

clean: 
	rm *.out *.o


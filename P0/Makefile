# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
all: p0.c types.h shell.h historicList.h libp0.h filesList.h shell.o historicList.o libp0.o filesList.o
	$(CC) $(CFLAGS) -o p0.out p0.c shell.o historicList.o libp0.o filesList.o

shell.o: types.h shell.h libp0.h shell.c
	$(CC) $(CFLAGS) -c shell.c

libp0.o: types.h libp0.h libp0.c
	$(CC) $(CFLAGS) -c libp0.c

filesList.o: types.h filesList.h filesList.c
	$(CC) $(CFLAGS) -c filesList.c

historicList.o: types.h historicList.h historicList.c
	$(CC) $(CFLAGS) -c historicList.c

clean: 
	rm *.out *.o


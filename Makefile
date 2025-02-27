# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
all: p3.c types.h shell.h historicList.h libp0.h filesList.h libp1.h libp2.h libp3.h memoryList.h backgroundList.h pathList.h vars.c shell.o historicList.o libp0.o filesList.o libp1.o libp2.o libp3.o memoryList.o backgroundList.o pathList.o
	$(CC) $(CFLAGS) -o p3 p3.c shell.o historicList.o libp0.o filesList.o libp1.o libp2.o libp3.o memoryList.o vars.c backgroundList.o pathList.o

shell.o: types.h shell.h libp0.h shell.c
	$(CC) $(CFLAGS) -c shell.c

libp0.o: types.h libp0.h libp0.c
	$(CC) $(CFLAGS) -c libp0.c

libp1.o: types.h libp1.h libp1.c
	$(CC) $(CFLAGS) -c libp1.c

libp2.o: types.h libp2.h libp2.c vars.c
	$(CC) $(CFLAGS) -c libp2.c vars.c

libp3.o: types.h libp3.h libp3.c
	$(CC) $(CFLAGS) -c libp3.c

filesList.o: types.h filesList.h filesList.c
	$(CC) $(CFLAGS) -c filesList.c

historicList.o: types.h historicList.h historicList.c
	$(CC) $(CFLAGS) -c historicList.c

memoryList.o: types.h memoryList.h memoryList.c
	$(CC) $(CFLAGS) -c memoryList.c

backgroundList.o: types.h backgroundList.h backgroundList.c
	$(CC) $(CFLAGS) -c backgroundList.c

pathList.o: types.h pathList.h pathList.c
	$(CC) $(CFLAGS) -c pathList.c

clean: 
	rm p3 *.o


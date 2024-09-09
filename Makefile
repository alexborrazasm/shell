p0.out: main.c shell shell.h shell.c
	gcc -o p0.out main.c shell.o

shell.o: shell.h shell.c 
	gcc -c shell.o shell.c

p0.o: p0.h p0.c
	gcc -c p0.o p0.c

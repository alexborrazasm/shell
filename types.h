#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>

#define MAX_BUFFER_INPUT 50
#define MAX_ARGS 10
#define UNUSED(expr) do { (void)(expr); } while (0)

typedef struct tArgs {
    char* array[MAX_ARGS];
    int len;
} tArgs;

// Forward declaration for tListH
typedef struct tNode *tPosH;
typedef tPosH tListH;


typedef struct tLists {
    tListH historic;
} tLists;

typedef const struct tCommand 
{
    const char* name;
    void (*func)(tArgs, tLists);
} tCommand;

#endif
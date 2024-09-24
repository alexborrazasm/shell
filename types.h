#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_BUFFER_INPUT 50
#define MAX_ARGS 10
#define UNUSED(expr) do { (void)(expr); } while (0)

typedef struct tArgs
{
    char *array[MAX_ARGS];
    int len;
} tArgs;

// Forward declaration for tListH
typedef struct tNode *tPosH;
typedef tPosH tListH;

// Forward declaration for tListH
typedef struct tNode *tPosF;
typedef tPosF tListF;

typedef struct tLists
{
    tListH historic;
    tListF files;
} tLists;
// End forward declaration for tListH

typedef struct tCommand
{
    const char *name;
    void (*func)(tArgs, tLists);
    const char *help0;
    const char *help1;
} tCommand;

#endif
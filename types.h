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
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

#define MAX_BUFFER_INPUT 256
#define MAX_ARGS 30
#define UNUSED(expr) do { (void)(expr); } while (0)
#define MODE_NULL -1

// Colors
#define RST         "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define RED_BOLD    "\033[1m\033[31m"
#define GREEN_BOLD  "\033[1m\033[32m"
#define BLUE_BOLD   "\033[1m\033[34m"

typedef unsigned char byte;

typedef struct tArgsMain
{
    int argc;
    char **argv, **envp;
} tArgsMain;

typedef struct tArgs
{
    char *array[MAX_ARGS];
    int len;
    tArgsMain main;
} tArgs;

// Forward declaration for tListH
typedef struct tNodeH *tPosH;
typedef tPosH tListH;

// Forward declaration for tListF
typedef struct tNodeF *tPosF;
typedef tPosF tListF;

// Forward declaration for tListM
typedef struct tNodeM *tPosM;
typedef tPosM tListM;

// Forward declaration for tListB
typedef struct tNodeB *tPosB;
typedef tPosB tListB;

// Forward declaration for tListP
typedef struct tNodeP *tPosP;
typedef tPosP tListP;

typedef struct tLists
{
    tListH historic;
    tListF files;
    tListM memory;
    tListB backgroud;
    tListP path;
} tLists;
// End forward declaration for tListH

typedef struct tCommand
{
    const char *name;
    void (*func)(tArgs, tLists*);
    const char *help0;
    const char *help1;
} tCommand;

#endif
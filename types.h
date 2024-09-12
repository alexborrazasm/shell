#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_BUFFER_INPUT 50
#define MAX_ARGS 10
#define UNUSED(expr) do { (void)(expr); } while (0)


typedef struct tArgs 
{
    char* array[MAX_ARGS];
    int len;
} tArgs;

// Start historyList types
typedef struct tItemH
{
    char command[MAX_BUFFER_INPUT];
} tItemH;

typedef struct tNode *tPosH;

struct tNode {
    tItemH data;
    tPosH next;
    tPosH prev;
};

typedef tPosH tListH;

typedef struct tCommand 
{
    const char* name;
    void (*func)(tArgs, tListH);
} tCommand;

// End historyList types
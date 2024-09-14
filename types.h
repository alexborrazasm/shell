#include "historicList.h"

typedef struct tArgs
{
    char* array[MAX_ARGS];
    int len;
} tArgs;

typedef const struct tCommand 
{
    const char* name;
    void (*func)(tArgs, tListH);
} tCommand;

typedef struct tLists
{
    tListH historic;
} tLists;
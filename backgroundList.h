#ifndef BACKGROUND_LIST_H
#define BACKGROUND_LIST_H

#include "types.h"

#define BNULL NULL
#define S_ACTIVE   (1 << 0)  // 0001 (1)
#define S_FINISHED (1 << 1)  // 0010 (2)
#define S_SIGNALED (1 << 2)  // 0100 (4)
#define S_STOPPED  (1 << 4)  // 1000 (8)
#define S_ACTIVE_STOPED (S_ACTIVE | S_STOPPED) // 1100 (12)

typedef struct tItemB
{
    int pid;
    time_t time;
    byte status;
    int sin;
    char commandLine[MAX_BUFFER_INPUT];
} tItemB;

struct tNodeB
{
    tItemB data;
    struct tNodeB *next;
    struct tNodeB *prev;
};

typedef tPosB tListB;

void createEmptyListB(tListB *L);

bool isEmptyListB(tListB L);

tPosB firstB(tListB L);

tPosB lastB(tListB L);

tPosB nextB(tPosB p, tListB L);

tPosB previousB(tPosB p, tListB L);

bool insertItemB(tItemB d, tPosB p, tListB *L);

void deleteAtPositionB(tPosB p, tListB *L);

tItemB getItemB(tPosB p, tListB L);

void updateItemB(tItemB d, tPosB p, tListB* L);

#endif // BACKGROUND_LIST_H
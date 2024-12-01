#ifndef BACKGROUND_LIST_H
#define BACKGROUND_LIST_H

#include "types.h"

#define BNULL NULL

typedef struct tItemB
{
    int TODO;
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

// TODO FIND

#endif // BACKGROUND_LIST_H
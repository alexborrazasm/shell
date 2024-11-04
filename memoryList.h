#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H

#include "types.h"

#define MNULL NULL

typedef struct tItemM
{
    byte type;
    void* address;
    size_t size;
    time_t date;
    char name[MAX_BUFFER_INPUT];
    int n; // Key or df
} tItemM;


struct tNodeM
{
    tItemM data;
    struct tNodeM *next;
    struct tNodeM *prev;
};

typedef tPosM tListM;

void createEmptyListM(tListM *L);

bool isEmptyListM(tListM L);

tPosM firstM(tListM L);

tPosM lastM(tListM L);

tPosM nextM(tPosM p, tListM L);

tPosM previousM(tPosM p, tListM L);

bool insertItemM(tItemM d, tPosM p, tListM *L);

void deleteAtPositionM(tPosM p, tListM *L);

tItemM getItemM(tPosM p, tListM L);

#endif // MEMORY_LIST_H
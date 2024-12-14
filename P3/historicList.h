#ifndef HISTORIC_LIST_H
#define HISTORIC_LIST_H

#include "types.h"

#define HNULL NULL

typedef struct tItemH
{
    char command[MAX_BUFFER_INPUT];
    int n;
} tItemH;

struct tNodeH
{
    tItemH data;
    struct tNodeH *next;
    struct tNodeH *prev;
};

typedef tPosH tListH;

void createEmptyListH(tListH *L);

bool isEmptyListH(tListH L);

tPosH firstH(tListH L);

tPosH lastH(tListH L);

tPosH nextH(tPosH p, tListH L);

tPosH previousH(tPosH p, tListH L);

bool insertItemH(tItemH d, tPosH p, tListH *L);

void deleteAtPositionH(tPosH p, tListH *L);

tItemH getItemH(tPosH p, tListH L);

#endif // HISTORIC_LIST_H
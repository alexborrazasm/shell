#ifndef HISTORIC_LIST_H
#define HISTORIC_LIST_H

#include "types.h"

#define LNULL NULL

typedef struct tItemH
{
    char command[MAX_BUFFER_INPUT];
    int n;
} tItemH;

struct tNode
{
    tItemH data;
    struct tNode *next;
    struct tNode *prev;
};

typedef tPosH tListH;

void createEmptyListF(tListH *L);

bool isEmptyListF(tListH L);

tPosH firstF(tListH L);

tPosH lastF(tListH L);

tPosH nextF(tPosH p, tListH L);

tPosH previousF(tPosH p, tListH L);

bool insertItemF(tItemH d, tPosH p, tListH *L);

void deleteAtPositionF(tPosH p, tListH *L);

tItemH getItemF(tPosH p, tListH L);

#endif
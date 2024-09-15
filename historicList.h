#ifndef HISTORIC_LIST_H
#define HISTORIC_LIST_H

#include "types.h"

#define LNULL NULL

typedef struct tItemH {
    char command[MAX_BUFFER_INPUT];
    int n;
} tItemH;

struct tNode {
    tItemH data;
    struct tNode *next;
    struct tNode *prev;
};

typedef tPosH tListH;

void createEmptyList(tListH* L);

bool isEmptyList(tListH L);

tPosH first(tListH L);

tPosH last(tListH L);

tPosH next(tPosH p, tListH L);

tPosH previous(tPosH p, tListH L);

bool insertItem(tItemH d, tPosH p, tListH* L);

void deleteAtPosition(tPosH p, tListH* L);

tItemH getItem(tPosH p, tListH L);

#endif
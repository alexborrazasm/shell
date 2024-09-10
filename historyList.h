#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

#define LNULL NULL

typedef struct tNode *tPosH;

struct tNode {
    tItemH data;
    tPosH next;
    tPosH prev;
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
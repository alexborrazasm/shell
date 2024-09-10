#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

typedef struct tNode *tPosH;

struct tNode {
    tItemH data;
    tPosH next;
    tPosH prev;
};

typedef tPosH tList;

void createEmptyList(tList* L);

bool isEmptyList(tList L);

tPosH first(tList L);

tPosH last(tList L);

tPosH next(tPosH p, tList L);

tPosH previous(tPosH p, tList L);

bool insertItem(tItemH d, tPosH p, tList* L);

void deleteAtPosition(tPosH p, tList* L);

tItemH getItem(tPosH p, tList L);
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

typedef struct tNode *tPosL;

struct tNode {
    tItemL data;
    tPosL next;
    tPosL prev;
};

typedef tPosL tList;

void createEmptyList(tList* L);

bool isEmptyList(tList L);

tPosL first(tList L);

tPosL last(tList L);

tPosL next(tPosL p, tList L);

tPosL previous(tPosL p, tList L);

bool insertItem(tItemL d, tPosL p, tList* L);

void deleteAtPosition(tPosL p, tList* L);

tItemL getItem(tPosL p, tList L);
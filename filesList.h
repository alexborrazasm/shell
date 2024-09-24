#ifndef FILES_LIST_H
#define FILES_LIST_H

#include "types.h"

#define LNULL NULL

typedef struct tItemF
{
    char info[MAX_BUFFER_INPUT];
    int df;
    char mode[MAX_BUFFER_INPUT];
} tItemF;

struct tNodeF
{
    tItemF data;
    struct tNodeF *next;
    struct tNodeF *prev;
};

typedef tPosF tListF;

void createEmptyListF(tListF *L);

bool isEmptyListF(tListF L);

tPosF firstF(tListF L);

tPosF lastF(tListF L);

tPosF nextF(tPosF p, tListF L);

tPosF previousF(tPosF p, tListF L);

bool insertItemF(tItemF d, tPosF p, tListF *L);

void deleteAtPositionF(tPosF p, tListF *L);

tItemF getItemF(tPosF p, tListF L);


bool updateItemF(tPosF p,tItemF I);
#endif
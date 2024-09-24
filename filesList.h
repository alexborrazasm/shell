#ifndef FILES_LIST_H
#define FILES_LIST_H

#include "types.h"

#define FNULL NULL

typedef struct tItemF
{
    int df;
    char info[MAX_BUFFER_INPUT];
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

void updateItemF(tItemF d, tPosF p, tListF* L);

tPosF findItemF(int df, tListF L);

#endif
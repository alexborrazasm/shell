#ifndef PATH_LIST_H
#define PATH_LIST_H

#include "types.h"

#define PNULL NULL

typedef char* tItemP;

struct tNodeP
{
    tItemP data;
    struct tNodeP *next;
    struct tNodeP *prev;
};

typedef tPosP tListP;

void createEmptyListP(tListP *L);

bool isEmptyListP(tListP L);

tPosP firstP(tListP L);

tPosP lastP(tListP L);

tPosP nextP(tPosP p, tListP L);

tPosP previousP(tPosP p, tListP L);

bool insertItemP(tItemP d, tPosP p, tListP *L);

void deleteAtPositionP(tPosP p, tListP *L);

tItemP getItemP(tPosP p, tListP L);

tPosP findItemP(char* path, tListP L);

#endif // PATH_LIST_H
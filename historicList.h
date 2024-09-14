#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>

#define MAX_BUFFER_INPUT 50
#define MAX_ARGS 10
#define UNUSED(expr) do { (void)(expr); } while (0)

#define LNULL NULL

typedef struct tItemH
{
    char command[MAX_BUFFER_INPUT];
    int n;
} tItemH;

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
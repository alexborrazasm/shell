#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H

#include "types.h"

#define MNULL NULL

#define M_MALLOC  (1 << 0)  // 0001 (1)
#define M_MMAP    (1 << 1)  // 0010 (2)
#define M_SHARED  (1 << 2)  // 0100 (4)
#define M_ALL     (M_MALLOC | M_MMAP | M_SHARED)  // 0111 (7)

typedef struct tItemM
{
    byte type;
    void* address;
    size_t size;
    time_t date;
    char name[MAX_BUFFER_INPUT];
    int keyDF; // Key or df
} tItemM;


struct tNodeM
{
    tItemM data;
    struct tNodeM *next;
    struct tNodeM *prev;
};

typedef tPosM tListM;

void createEmptyListM(tListM *L);

bool isEmptyListM(tListM L);

tPosM firstM(tListM L);

tPosM lastM(tListM L);

tPosM nextM(tPosM p, tListM L);

tPosM previousM(tPosM p, tListM L);

bool insertItemM(tItemM d, tPosM p, tListM *L);

void deleteAtPositionM(tPosM p, tListM *L);

tItemM getItemM(tPosM p, tListM L);

// Returns the position of the first match with the size
tPosM findMalloc(size_t size, tListM L);

// Find by memory address
tPosM findByAddress(void* address, tListM L);

// Returns the position of the first match with the filename
tPosM findMMap(char* name, tListM L);

// Returns the position of the first match with the shared key
tPosM findShared(int key, tListM L);

#endif // MEMORY_LIST_H
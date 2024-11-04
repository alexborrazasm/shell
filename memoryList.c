#include "memoryList.h"

bool createNodeM(tPosM *p);

void createEmptyListM(tListM *L)
{
    *L = MNULL;
}

bool isEmptyListM(tListM L)
{
    return L == MNULL;
}

tPosM firstM(tListM L)
{
    return L;
}

tPosM lastM(tListM L)
{
    return L->prev;
}

tPosM previousM(tPosM p, tListM L)
{
    if (p->prev == lastM(L))
    {
        return MNULL;
    }
    else
        return p->prev;
}

tPosM nextM(tPosM p, tListM L)
{
    UNUSED(L);
    return p->next;
}

bool insertItemM(tItemM d, tPosM p, tListM *L)
{
    tPosM q;

    if (!createNodeM(&q))
        return false;

    q->data = d;
    q->next = MNULL;
    q->prev = MNULL;

    if (isEmptyListM(*L)) // Empty list
    {
        *L = q; // The first node its q
        q->prev = q;
    }
    else if (p == MNULL) // Insert at the end
    {
        tPosM r;

        r = (*L)->prev; // (*L)->prev is the last node
        r->next = q;
        q->prev = r;
        (*L)->prev = q;
    }
    else if (p == *L) // Insert at first position
    {
        q->next = *L;
        q->prev = (*L)->prev; // (*L)->prev point to last node
        (*L)->prev = q;
        *L = q;
    }
    else // Insert in the middle
    {
        // Pointers are reassigned
        q->next = p;
        q->prev = p->prev;
        p->prev->next = q;
        p->prev = q;
    }
    return true;
}

void deleteAtPositionM(tPosM p, tListM *L)
{
    tPosM q;

    if (p == *L) // p is the first node
    {
        if (p->next == MNULL)
        {
            *L = MNULL;
        }
        else
        {
            *L = p->next;         // point the list to second node
            (*L)->prev = p->prev; // p->prev point to last node
        }
    }
    else if (p->next == MNULL) // p is the last node
    {
        q = p->prev;
        (*L)->prev = q;
        q->next = MNULL;
    }
    else // p is in the middle
    {
        q = p->next;
        p->prev->next = q;
        q->prev = p->prev;
    }

    // Free memory
    free(p);
}

tItemM getItemM(tPosM p, tListM L)
{
    UNUSED(L);
    return (p->data);
}

bool createNodeM(tPosM *p)
{
    *p = malloc(sizeof(struct tNodeM));
    if (*p == MNULL)
        return false;
    else
        return true;
}
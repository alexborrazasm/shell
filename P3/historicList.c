#include "historicList.h"

bool createNodeH(tPosH *p);

void createEmptyListH(tListH *L)
{
    *L = HNULL;
}

bool isEmptyListH(tListH L)
{
    return L == HNULL;
}

tPosH firstH(tListH L)
{
    return L;
}

tPosH lastH(tListH L)
{
    return L->prev;
}

tPosH previousH(tPosH p, tListH L)
{
    if (p->prev == lastH(L))
    {
        return HNULL;
    }
    else
        return p->prev;
}

tPosH nextH(tPosH p, tListH L)
{
    UNUSED(L);
    return p->next;
}

bool insertItemH(tItemH d, tPosH p, tListH *L)
{
    tPosH q;

    if (!createNodeH(&q))
        return false;

    q->data = d;
    q->next = HNULL;
    q->prev = HNULL;

    if (isEmptyListH(*L)) // Empty list
    {
        *L = q; // The first node its q
        q->prev = q;
    }
    else if (p == HNULL) // Insert at the end
    {
        tPosH r;

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

void deleteAtPositionH(tPosH p, tListH *L)
{
    tPosH q;

    if (p == *L) // p is the first node
    {
        if (p->next == HNULL)
        {
            *L = HNULL;
        }
        else
        {
            *L = p->next;         // point the list to second node
            (*L)->prev = p->prev; // p->prev point to last node
        }
    }
    else if (p->next == HNULL) // p is the last node
    {
        q = p->prev;
        (*L)->prev = q;
        q->next = HNULL;
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

tItemH getItemH(tPosH p, tListH L)
{
    UNUSED(L);
    return (p->data);
}

bool createNodeH(tPosH *p)
{
    *p = malloc(sizeof(struct tNodeH));
    if (*p == HNULL)
        return false;
    else
        return true;
}
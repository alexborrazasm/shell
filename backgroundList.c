#include "backgroundList.h"

bool createNodeB(tPosB *p);

void createEmptyListB(tListB *L)
{
    *L = BNULL;
}

bool isEmptyListB(tListB L)
{
    return L == BNULL;
}

tPosB firstB(tListB L)
{
    return L;
}

tPosB lastB(tListB L)
{
    return L->prev;
}

tPosB previousB(tPosB p, tListB L)
{
    if (p->prev == lastB(L))
    {
        return BNULL;
    }
    else
        return p->prev;
}

tPosB nextB(tPosB p, tListB L)
{
    UNUSED(L);
    return p->next;
}

bool insertItemB(tItemB d, tPosB p, tListB *L)
{
    tPosB q;

    if (!createNodeB(&q))
        return false;

    q->data = d;
    q->next = BNULL;
    q->prev = BNULL;

    if (isEmptyListB(*L)) // Empty list
    {
        *L = q; // The first node its q
        q->prev = q;
    }
    else if (p == BNULL) // Insert at the end
    {
        tPosB r;

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

void deleteAtPositionB(tPosB p, tListB *L)
{
    tPosB q;

    if (p == *L) // p is the first node
    {
        if (p->next == BNULL)
        {
            *L = BNULL;
        }
        else
        {
            *L = p->next;         // point the list to second node
            (*L)->prev = p->prev; // p->prev point to last node
        }
    }
    else if (p->next == BNULL) // p is the last node
    {
        q = p->prev;
        (*L)->prev = q;
        q->next = BNULL;
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

tItemB getItemB(tPosB p, tListB L)
{
    UNUSED(L);
    return (p->data);
}

bool createNodeB(tPosB *p)
{
    *p = malloc(sizeof(struct tNodeB));
    if (*p == BNULL)
        return false;
    else
        return true;
}

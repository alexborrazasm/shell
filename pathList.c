#include "pathList.h"

bool createNodeP(tPosP *p);

void createEmptyListP(tListP *L)
{
    *L = PNULL;
}

bool isEmptyListP(tListP L)
{
    return L == PNULL;
}

tPosP firstP(tListP L)
{
    return L;
}

tPosP lastP(tListP L)
{
    return L->prev;
}

tPosP previousP(tPosP p, tListP L)
{
    if (p->prev == lastP(L))
    {
        return PNULL;
    }
    else
        return p->prev;
}

tPosP nextP(tPosP p, tListP L)
{
    UNUSED(L);
    return p->next;
}

bool insertItemP(tItemP d, tPosP p, tListP *L)
{
    tPosP q;

    if (!createNodeP(&q))
        return false;

    q->data = d;
    q->next = PNULL;
    q->prev = PNULL;

    if (isEmptyListP(*L)) // Empty list
    {
        *L = q; // The first node its q
        q->prev = q;
    }
    else if (p == PNULL) // Insert at the end
    {
        tPosP r;

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

void deleteAtPositionP(tPosP p, tListP *L)
{
    tPosP q;

    if (p == *L) // p is the first node
    {
        if (p->next == PNULL)
        {
            *L = PNULL;
        }
        else
        {
            *L = p->next;         // point the list to second node
            (*L)->prev = p->prev; // p->prev point to last node
        }
    }
    else if (p->next == PNULL) // p is the last node
    {
        q = p->prev;
        (*L)->prev = q;
        q->next = PNULL;
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

tItemP getItemP(tPosP p, tListP L)
{
    UNUSED(L);
    return (p->data);
}

bool createNodeP(tPosP *p)
{
    *p = malloc(sizeof(struct tNodeP));
    if (*p == PNULL)
        return false;
    else
        return true;
}

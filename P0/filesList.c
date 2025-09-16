#include "filesList.h"

bool createNodeF(tPosF *p);

void createEmptyListF (tListF *L)
{
    *L = FNULL;
}

bool isEmptyListF(tListF L)
{
    return L == FNULL;
}

tPosF firstF(tListF L)
{
    return L;
}

tPosF lastF(tListF L)
{
    return L->prev;
}

tPosF previousF(tPosF p, tListF L)
{
    if (p->prev == lastF(L))
    {
        return FNULL;
    }
    else
        return p->prev;
}

tPosF nextF(tPosF p, tListF L)
{
    UNUSED(L);
    return p->next; // Devuelve NULL si es el último, y si no el siguiente
}

bool insertItemF(tItemF d, tPosF p, tListF *L)
{
    tPosF q;

    if (!createNodeF(&q))
        return false;

    q->data = d;
    q->next = FNULL;
    q->prev = FNULL;

    if (isEmptyListF(*L)) // Empty list
    {
        *L = q; // The first node its q
        q->prev = q;
    }
    else if (p == FNULL) // Insert at the end
    {
        tPosF r;

        r = (*L)->prev; // (*L)->prev is the lastF node
        r->next = q;
        q->prev = r;
        (*L)->prev = q;
    }
    else if (p == *L) // Insert at first position
    {
        q->next = *L;
        q->prev = (*L)->prev; // (*L)->prev point to lastF node
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

void deleteAtPositionF(tPosF p, tListF *L)
{
    tPosF q;

    if (p == *L) // p is the first node
    {
        if (p->next == FNULL)
        {
            *L = FNULL;
        }
        else
        {
            *L = p->next;         // point the list to second node
            (*L)->prev = p->prev; // p->prev point to lastF node
        }
    }
    else if (p->next == FNULL) // p is the lastF node
    {
        q = p->prev;
        (*L)->prev = q;
        q->next = FNULL;
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

tItemF getItemF(tPosF p, tListF L)
{
    UNUSED(L);
    return (p->data);
}

bool createNodeF(tPosF *p)
{
    *p = malloc(sizeof(struct tNodeF));
    if (*p == FNULL)
        return false;
    else
        return true;
}

void updateItemF(tItemF d, tPosF p, tListF* L)
{
    UNUSED(L);
    p->data=d;
}

tPosF findItemF(int df, tListF L)
{
    tPosF p;

    for (p = L; (p != FNULL) && (p->data.df != df); p = p->next);
    
    return p;
}

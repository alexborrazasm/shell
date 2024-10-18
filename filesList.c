#include "filesList.h"

tPosF findPosition(tListF L, tItemF d);

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
    return p->next;
}

tPosF findPosition(tListF L, tItemF d)
{
    tPosF p;

    p = L;
    while ((p->next != FNULL) && (p->next->data.df < d.df))
        p = p->next;

    return p->next;
}

bool insertItemF(tItemF d, tListF *L)
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
    else if (d.df < (*L)->data.df) // d is less than all items in the list 
    {   // Inset at first position
        q->next = *L;
        q->prev = (*L)->prev; // (*L)->prev point to lastF node
        (*L)->prev = q;
        *L = q;
    }
    else  // Find and insert in the middle
    {
        tPosF p = findPosition(*L, d);
        
        
        if (p == FNULL) // Insert at end position
        {
            tPosF last = (*L)->prev;
            last->next = q;
            q->prev = last;
            (*L)->prev = q; // (*L)->prev point to lastF node
        }
        
        else // Insert int the middle
        {
            // Pointers are reassigned
            q->next = p;
            q->prev = p->prev;
            p->prev->next = q;
            p->prev = q;
        }
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
        createEmptyListF(L);
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

tPosF findItemF(int df, tListF L) {
    tPosF p;

    for (p = L; (p != FNULL) && (p->data.df < df); p = p->next);
    
    // Check if the item has been found
    if ((p != FNULL) && (p->data.df == df)) {
        return p; // Item found
    }
    
    return FNULL; // Item not found
}

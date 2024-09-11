#include "historyList.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

bool createNode(tPosH* p);

void createEmptyList(tListH* L) 
{
    *L = LNULL;
}

bool isEmptyList(tListH L) 
{
    return L == LNULL;
}

tPosH first(tListH L) 
{
    return L;
}

tPosH last(tListH L) 
{
    return L->prev;
}

tPosH previous(tPosH p, tListH L) 
{
    UNUSED(L);
    return p->prev;
}

tPosH next(tPosH p, tListH L) 
{
    UNUSED(L);
    return p->next; // Devuelve NULL si es el último, y si no el siguiente
}

bool insertItem(tItemH d, tPosH p, tListH* L) 
{
    tPosH q;

    if (!createNode(&q)) 
        return false;
    
    q->data = d;
    q->next = LNULL;
    q->prev = LNULL;

    if (isEmptyList(*L)) // Empty list
    {  
        *L = q;  // The first node its q
        q->prev = q;
    } 
    else if (p == LNULL) // Insert at the end
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
    else  // Insert in the middle
    { 
        // Pointers are reassigned
        q->next = p;
        q->prev = p->prev;
        p->prev->next = q;
        p->prev = q;
    }
    return true;
}

void deleteAtPosition(tPosH p, tListH* L) 
{
    tPosH q;

    if (p == *L) // p is the first node
    {   
        if (p->next == LNULL)
        {
            *L = LNULL;
        }
        else 
        {
            *L = p->next; // point the list to second node
            (*L)->prev = p->prev; // p->prev point to last node
        }
    } 
    else if (p->next == LNULL) // p is the last node
    { 
        q = p->prev;
        (*L)->prev = q;
        q->next = LNULL;
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

tItemH getItem(tPosH p, tListH L) {
    UNUSED(L);
    return(p->data);
}

bool createNode(tPosH *p) 
{
    *p = malloc(sizeof(struct tNode));
    if (*p == LNULL) return false;
    else return true;           
}
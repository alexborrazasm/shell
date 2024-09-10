#include "historyList.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

bool createNode(tPosH* p);

void createEmptyList(tList* L) 
{
    *L = NULL;
}

bool isEmptyList(tList L) 
{
    return L == NULL;
}

tPosH first(tList L) 
{
    return L;
}

tPosH last(tList L) 
{
    if (isEmptyList(L)) return NULL;
    // Search last item
    tPosH q;
    for (q = L; q->next != NULL; q = q->next);
    return q;
}

tPosH previous(tPosH p, tList L) 
{
    UNUSED(L);
    return p->prev;
}

tPosH next(tPosH p, tList L) 
{
    UNUSED(L);
    return p->next; // Devuelve NULL si es el último, y si no el siguiente
}

bool insertItem(tItemH d, tPosH p, tList* L) 
{
    tPosH q;

    if (!createNode(&q)) 
        return false;
    
    q->data = d;
    q->next = NULL;
    q->prev = NULL;

    if (*L == NULL)//isEmptyList(*L)) // Empty list
    {  
        *L = q;  // The first node its q
    } 
    else if (p == NULL) // Insert at the end
    { 
        tPosH r;
        
        for (r = *L; r->next != NULL; r = r->next);

        r->next = q;
        q->prev = r;
    } 
    else if (p == *L) // Insert at first position
    { 
        q->next = *L;
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

void deleteAtPosition(tPosH p, tList* L) 
{
    tPosH r, q;

    if (p == *L) // p is the first node
    { 
        *L = p->next;
    } 
    else if (p->next == NULL) // p is the last node
    { 
        r = previous(p, *L);
        r->next = NULL;
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

tItemH getItem(tPosH p, tList L) {
    UNUSED(L);
    return(p->data);
}

bool createNode(tPosH *p) 
{
    *p = malloc(sizeof(struct tNode));
    if (*p==NULL) return false;
    else return true;           
}
#include "dynamicList.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

bool createNode(tPosL* p);

void createEmptyList(tList* L) 
{
    *L = NULL;
}

bool isEmptyList(tList L) 
{
    return L == NULL;
}

tPosL first(tList L) 
{
    return L;
}

tPosL last(tList L) 
{
    if (isEmptyList(L)) return NULL;
    // Search last item
    tPosL q;
    for (q = L; q->next != NULL; q = q->next);
    return q;
}

tPosL previous(tPosL p, tList L) 
{
    UNUSED(L);
    if (p == NULL) return NULL;
    return p->prev;
}

tPosL next(tPosL p, tList L) 
{
    UNUSED(L);
    if (p == NULL) return NULL;
    return p->next; // Devuelve NULL si es el último, y si no el siguiente
}

bool insertItem(tItemL d, tPosL p, tList* L) 
{
    tPosL q;

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
        tPosL r;
        
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

void deleteAtPosition(tPosL p, tList* L) 
{
    tPosL r, q;

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

tItemL getItem(tPosL p, tList L) {
    UNUSED(L);
    return(p->data);
}

bool createNode(tPosL *p) 
{
    *p = malloc(sizeof(struct tNode));
    if (*p==NULL) return false;
    else return true;           
}
#include "filesList.h"

bool createNodeF(tPosF *p);

void createEmptyListF(tListF *L)
{
    *L = LNULL;
}

void initFList(tListF L){
    tItemF imput;
    tItemF output;
    tItemF error;

    imput.df=0;
    strcpy(imput.info, "entrada estandar");
    strcpy(imput.info, "O_RDWR");

    output.df=1;
    strcpy(output.info, "entrada estandar");
    strcpy(output.info, "O_RDWR");

    error.df=2;
    strcpy(error.info, "entrada estandar");
    strcpy(error.info, "O_RDWR");

    insertItemF(imput, lastF(L), &L);
    insertItemF(output, lastF(L), &L);
    insertItemF(error, lastF(L), &L);
}

bool isEmptyListF(tListF L)
{
    return L == LNULL;
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
        return LNULL;
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
    q->next = LNULL;
    q->prev = LNULL;

    if (isEmptyListF(*L)) // Empty list
    {
        *L = q; // The first node its q
        q->prev = q;
    }
    else if (p == LNULL) // Insert at the end
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
        if (p->next == LNULL)
        {
            *L = LNULL;
        }
        else
        {
            *L = p->next;         // point the list to second node
            (*L)->prev = p->prev; // p->prev point to lastF node
        }
    }
    else if (p->next == LNULL) // p is the lastF node
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

tItemF getItemF(tPosF p, tListF L)
{
    UNUSED(L);
    return (p->data);
}

bool createNodeF(tPosF *p)
{
    *p = malloc(sizeof(struct tNodeF));
    if (*p == LNULL)
        return false;
    else
        return true;
}


bool updateItemF(tPosF p,tItemF I) {
    if (p == NULL) {
        return false;
    }
    
    p->data = I;
    
    return true;
}
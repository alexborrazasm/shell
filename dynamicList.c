#include "dynamicList.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

bool createNode(tPosL* p);
/* createNode (tPosL) -> tPosL, bool
 * Función constructora:
 *  Objetivo:
 *      Crear un nuevo nodo en una lista enlazada dinámica, asignando memoria para él.
 *  Entrada:
 *      p -> Puntero al nodo a crear.
 *  Salida:
 *      p -> Puntero al nodo creado.
 *      bool -> Verdadero si la creación del nodo fue exitosa, falso en caso contrario.
*/

void createEmptyList(tList* L) {
    *L=LNULL; // Inicializa la lista como vacía
}

bool isEmptyList(tList L) {
    return L==LNULL; // Devuelve L, siempre apunta al primer elemento de la lista
}

tPosL first(tList L) {
    return L; // Devuelve la primera posición de la lista
}

tPosL last(tList L) {
    tPosL q;
    // Itera hasta encontrar el último elemento de la lista
    for (q = L; q->next != LNULL; q = q->next);
    return q; // Devuelve la última posición de la lista
}

tPosL previous(tPosL p, tList L) {
    tPosL q;
    if (p==L) return LNULL;  // Si p es el primer elemento, devuelve LNULL
    else {
        for(q=L; q->next!=p; q=q->next); // Itera hasta encontrar el elemento
        return q;                        // anterior a p
    }
}

tPosL next(tPosL p, tList L) {
    UNUSED(L);
    return p->next; // Devuelve LNULL si es el último, y si no el siguiente
}

bool insertItem(tItemL d, tPosL p, tList* L) {
    tPosL q;
    if (!createNode(&q)) { // Crea un nuevo nodo para almacenar el elemento
        return false;         // En el caso de no poder crearlo,devuelve falso
    } else {
        q->data = d; // Asigna el valor del elemento al nuevo nodo
        q->next = LNULL; // Establece el siguiente nodo como LNULL, valor por defecto
        if (*L == LNULL) { // Lista vacía
            *L = q; // el nuevo nodo se convierte en el primero
        } else if (p == LNULL) { // Se inserta al final de la lista
                tPosL r;
                for (r = *L; r->next != LNULL; r = r->next){
                }
                r->next = q;
        } else if (p == *L) { // Inserta al inicio de la lista
            q->next = p;
            *L = q;
        } else { // Insertar en el medio, se intercambian los datos
            // Se "reasignan los punteros"
            q->next = p->next; // Inserta el nuevo nodo en una posición intermedia
            p->next = q;
            q->data = p -> data; // Intercambia los datos entre los nodos
            p->data = d;

        }
    }
    return true; // Devuelve true si la inserción fue exitosa
}

void deleteAtPosition(tPosL p, tList* L) {
    tPosL r,q;
    if (p == *L) { // p es el primer elemento
        *L = p->next; // Lo elimina
    } else if (p->next == LNULL) { // p está al final
        r=previous (p,*L); // Obtiene el elemento anterior a p
        r->next = LNULL; // Elimina el último elemento
    } else { // p posición intermedia
        q=p->next;
        p->next=q->next;
        p->data=q->data;
        p=q;
    }
    free(p); // Libera la memoria del nodo eliminado
}

tItemL getItem(tPosL p, tList L) {
    UNUSED(L);
    return(p->data); // Devuelve el dato almacenado en la posición p
}

void updateItem(tItemL d, tPosL p, tList* L) {
    UNUSED(L);
    p->data=d; // Actualiza el dato almacenado en la posición p
}

tPosL findItem(tUserName d, tList L) {
    if(isEmptyList(L))
        return LNULL;
    else {
        tPosL q;
        for(q = L; q != LNULL && strcmp(q->data.userName, d) != 0; q = q->next);
        return q; // Busca un elemento por el nombre de usuario y devuelve su posición
    }
}

bool createNode(tPosL *p){
    // Crear variable dinámica con el tamaño del struct tNode, inicializa el puntero
    *p = malloc(sizeof( struct tNode));
    if (*p==LNULL) return false;  // Si no se puede asignar memoria, devuelve false
    else return true;             // true si el nodo se crea correctamente
}
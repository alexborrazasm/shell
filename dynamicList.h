#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LNULL NULL

typedef char tCommand[10];

typedef struct tItemL {
    tCommand userName;
} tItemL;

typedef struct tNode *tPosL;

struct tNode {
    tItemL data;
    tPosL next;
};

typedef tPosL tList;

void createEmptyList(tList* L);
/* createEmptyList (tList) -> tList
 * Función constructora:
 *  Objetivo:
 *      Crea una lista vacía.
 *  Entrada:
 *      L -> Puntero a la lista a inicializar.
 *  Salida:
 *      L -> una lista vacía.
 *  Poscondición:
 *      La lista apuntada por L queda inicializada como vacía.
*/

bool isEmptyList(tList L);
/* isEmptyList (tList) -> bool
 * Función observadora:
 *  Objetivo:
 *      Determina si la lista está vacía o no.
 *  Entrada:
 *      L -> Lista a verificar.
 *  Salida:
 *      bool -> Verdadero si la lista está vacía, falso en caso contrario.
*/

tPosL first(tList L);
/* first (tList) -> tPosL
 * Función observadora:
 *  Objetivo:
 *      Devuelve la posición del primer elemento de la lista.
 *  Entrada:
 *      L -> Lista.
 *  Salida:
 *      tPosL -> Posición del primer elemento de la lista.
 *  Precondición:
 *      La lista no está vacía.
*/

tPosL last(tList L);
/* last (tList) -> tPosL
 * Función observadora:
 *  Objetivo:
 *      Devuelve la posición del último elemento de la lista.
 *  Entrada:
 *      L -> Lista a manipular.
 *  Salida:
 *      tPosL -> Posición del último elemento de la lista.
 *  Precondición:
 *      La lista no está vacía.
*/

tPosL next(tPosL p, tList L);
/* next (tPosL, tList) -> tPosL
 * Función observadora:
 *  Objetivo:
 *      Devuelve la posición del elemento siguiente al actual.
 *  Entrada:
 *      p -> Posición actual en la lista.
 *      L -> Lista.
 *  Salida:
 *      tPosL -> Posición del elemento siguiente o LNULL si es el último.
 *  Precondición:
 *      La posición p pertenece a la lista L.
*/

tPosL previous(tPosL p, tList L);
/* previous (tPosL, tList) -> tPosL
 * Función observadora:
 *  Objetivo:
 *      Devuelve la posición del elemento anterior al indicado.
 *  Entrada:
 *      p -> Posición actual en la lista.
 *      L -> Lista.
 *  Salida:
 *      tPosL -> Posición del elemento anterior al indicado.
 *  Precondición:
 *      La posición p pertenece a la lista L.
*/

bool insertItem(tItemL d, tPosL p, tList* L);
/* insertItem (tItemL, tPosL, tList) -> tList, bool
 * Función modificadora:
 *  Objetivo:
 *      Inserta un elemento en la posición indicada de la lista.
 *      Si la posición es LNULL, entonces se añade al final.
 *  Entrada:
 *      d -> Elemento a insertar.
 *      p -> Posición donde se insertará el elemento.
 *      L -> Puntero a la lista donde se realizará la inserción.
 *  Salida:
 *      L -> Lista con el elemento d insertado en la posición correspondiente.
 *      bool -> Verdadero si la inserción fue exitosa, falso en caso contrario.
 *  Precondición:
 *      La posición p pertenece a la lista L.
 *  Poscondición:
 *      El elemento d se inserta en la posición p de la lista L.
*/

void deleteAtPosition(tPosL p, tList* L);
/* deleteAtPosition (tPosL, tList) -> tList
 * Función modificadora:
 *  Objetivo:
 *      Elimina el elemento en la posición indicada de la lista.
 *  Entrada:
 *      p -> Posición del elemento a eliminar.
 *      L -> Puntero a la lista donde se realizará la eliminación.
 *  Salida:
 *      L -> Lista sin el elemento correspondiente a p.
 *  Precondición:
 *      La posición p pertenece a la lista L.
 *  Poscondición:
 *      Las posiciones de los elementos de la lista posteriores a p pueden haber variado.
*/

tItemL getItem(tPosL p, tList L);
/* getItem (tPosL, tList) -> tItemL
 * Función observadora:
 *  Objetivo:
 *      Devuelve el elemento en la posición indicada de la lista.
 *  Entrada:
 *      p -> Posición del elemento a obtener.
 *      L -> Lista a manipular.
 *  Salida:
 *      tItemL -> Elemento en la posición indicada de la lista.
 *  Precondición:
 *      La posición p pertenece a la lista L.
*/

void updateItem(tItemL d, tPosL p, tList* L);
/* updateItem (tItemL, tPosL, tList) -> tListL
 * Función modificadora:
 *  Objetivo:
 *      Actualiza el elemento en la posición indicada de la lista.
 *  Entrada:
 *      d -> Nuevo valor del elemento.
 *      p -> Posición del elemento a actualizar.
 *      L -> Puntero a la lista donde se realizará la actualización.
 *  Salida:
 *      L -> Lista con el contenido del elemento modificado.
 *  Precondición:
 *      La posición p pertenece a la lista L.
 *  Poscondición:
 *      El elemento en la posición p de la lista L es actualizado con el valor d.
*/

tPosL findItem(tUserName d, tList L);
/* findItem (tUserName, tList) -> tPosL
 * Función observadora:
 *  Objetivo:
 *      Devuelve la posición del primer elemento de la lista cuyo nombre de usuario se
 *      corresponda con el indicado (o LNULL si no existe tal elemento).
 *  Entrada:
 *      d -> Nombre de usuario a buscar.
 *      L -> Lista.
 *  Salida:
 *      tPosL -> Posición del elemento con el nombre de usuario indicado.
*/
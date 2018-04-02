// Esta linea sirve para que el código de este archivo solo se importe una vez
#include "../process/process.h"
#pragma once



/** Estructura de una Queue */
struct Queue_list
{
  process** array;
  unsigned int count;
  unsigned int size;
};

// Aquí le estoy poniendo un nombre más simple a la lista para no tener que
// referirme a la lista como struct Queue_list
/** Estructura de una Queue */
typedef struct Queue_list Queue;


//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

//OJO: No se debe modificar nada de esto

/** Crea una Queue inicialmente vacia y retorna su puntero */
Queue* Queue_init();

/** Inserta un elemento al final de la Queue */
void Queue_append(Queue* list, process* element);

/** Inserta el elemento dado en la posicion indicada */
void Queue_insert(Queue* list, process* element, unsigned int position);

/** Elimina el elemento de la posicion indicada y lo retorna */
process* Queue_delete(Queue* list, unsigned int position);

/** Elminia el primer elemento y lo retorna */
process* Queue_pop(Queue* list);

/** Retorna el valor del elemento en la posicion dada */
process* Queue_get(Queue* list, unsigned int position);

/** Concatena la segunda Queue a la primera Queue */
void Queue_concatenate(Queue* list1, Queue* list2);

/** Libera todos los recursos asociados a la lista */
void Queue_destroy(Queue* list);

// Importo el archivo Queue.h
#include "queue.h"
#include "../process/process.h"

// Libreria estandar de C
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

int arraySize = 8;

void upgradeSize(Queue* list)
{
  list->size *= 2;
  list->array = realloc(list->array, sizeof(process) * list->size);
}

void insertElement(Queue* list, int position, process* element)
{
  for (size_t i = list->count-1; i >= position; i--) {
    process* temp = list->array[i];
    list->array[i+1] = temp;
  }
  list->array[position] = element;
}

/** Crea una Queue inicialmente vacia y retorna su puntero */
Queue* Queue_init()
{
  Queue* list = malloc(sizeof(Queue));
  list->array = malloc(sizeof(int) * arraySize);
  list->size = arraySize;
  list->count = 0;
  return list;
}

void Queue_set_quantum(Queue* list, unsigned int q)
{
  list->q = q;
}

/** Inserta un elemento al final de la Queue */
void Queue_append(Queue* list, process* element)
{
  if (list->count>=list->size) {
    upgradeSize(list);
  }
  list->array[list->count] = element;
  list->count += 1;
}

/** Inserta el elemento dado en la posicion indicada */
void Queue_insert(Queue* list, process* element, unsigned int position)
{
  if (list->count>=list->size) {
    upgradeSize(list);
  }
  insertElement(list, position, element);
  list->count += 1;
}

process* Queue_pop(Queue* list)
{
  return Queue_delete(list, 0);
}

/** Elimina el elemento de la posicion indicada y lo retorna */
process* Queue_delete(Queue* list, unsigned int position)
{
  process *element = list->array[position];
  for (size_t i = position; i < list->count; i++) {
    list->array[i] = list->array[i+1];
  }
  list->count -= 1;
  return element;
}

/** Retorna el valor del elemento en la posicion dada */
process* Queue_get(Queue* list, unsigned int position)
{
  return list->array[position];
}

process* Queue_get_process_by_start_time(Queue* list, unsigned int start_time)
{
  process* p;
  for (size_t i = 0; i < list->count; i++) {
    p = list->array[i];
    if (p->start == start_time) {
      Queue_delete(list, i);
      return p;
    }
  }
  return NULL;
}

/** Concatena la segunda Queue a la primera Queue. No destruye la segunda Queue */
void Queue_concatenate(Queue* list1, Queue* list2)
{
  if (list1->count + list2->count <= list1->size)
  {
    for (size_t i = 0; i < list2->count; i++) {
      list1->array[list1->count+i] = list2->array[i];
    }
    list1->count += list2->count;
  } else {
    upgradeSize(list1);
    Queue_concatenate(list1, list2);
  }
}

/** Retorna verdadero si la lista esta vacia */
int Queue_isempty(Queue* list)
{
  if (list->count == 0){
    return 1;
  }
  return 0;
}

/** Libera todos los recursos asociados a la lista */
void Queue_destroy(Queue* list)
{
  free(list->array);
  free(list);
}

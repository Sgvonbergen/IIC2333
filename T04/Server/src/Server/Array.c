// Importo el archivo Array.h
#include "Array.h"

// Libreria estandar de C
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

int arraySize = 8;

void upgradeSize(Array* list)
{
  list->size *= 2;
  list->array = realloc(list->array, sizeof(int) * list->size);
}

void insertElement(Array* list, int position, int element)
{
  for (size_t i = list->count-1; i >= position; i--) {
    int temp = list->array[i];
    list->array[i+1] = temp;
  }
  list->array[position] = element;
}

/** Crea una Array inicialmente vacia y retorna su puntero */
Array* Array_init()
{
  Array* list = malloc(sizeof(Array));
  list->array = malloc(sizeof(int) * arraySize);
  list->size = arraySize;
  list->count = 0;
  return list;
}

/** Inserta un elemento al final del Array */
void Array_append(Array* list, int element)
{
  if (list->count>=list->size) {
    upgradeSize(list);
  }
  list->array[list->count] = element;
  list->count += 1;
}

/** Elimina el elemento de la posicion indicada y lo retorna */
int Array_delete(Array* list, unsigned int position)
{
  int element = list->array[position];
  for (size_t i = position; i < list->count; i++) {
    list->array[i] = list->array[i+1];
  }
  list->count -= 1;
  return element;
}

/** Retorna el valor del elemento en la posicion dada */
int Array_get(Array* list, unsigned int position)
{
  return list->array[position];
}

/** Libera todos los recursos asociados a la lista */
void Array_destroy(Array* list)
{
  free(list->array);
  free(list);
}

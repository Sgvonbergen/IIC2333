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

void insertElement(Array* list, int position, Client element)
{
  for (size_t i = list->count-1; i >= position; i--) {
    Client temp = list->array[i];
    list->array[i+1] = temp;
  }
  list->array[position] = element;
}

/** Crea una Array inicialmente vacia y retorna su puntero */
Array* Array_init()
{
  Array* list = malloc(sizeof(Array));
  list->array = malloc(sizeof(Client) * arraySize);
  list->size = arraySize;
  list->count = 0;
  return list;
}

/** Inserta un elemento al final del Array */
void Array_append(Array* list, Client element)
{
  if (list->count>=list->size) {
    upgradeSize(list);
  }
  list->array[list->count] = element;
  list->count += 1;
}

/** Elimina el elemento de la posicion indicada y lo retorna */
Client Array_delete(Array* list, unsigned int position)
{
  Client element = list->array[position];
  for (size_t i = position; i < list->count; i++) {
    list->array[i] = list->array[i+1];
  }
  list->count -= 1;
  return element;
}

void Array_delete_by_socket_id(Array* list, int socket_id)
{
  for (size_t i = 0; i < list->count; i++) {
    if (Array_get(list, i).socket_id == socket_id) {
      Array_delete(list, i);
      break;
    }
  }
}

/** Retorna el valor del elemento en la posicion dada */
Client Array_get(Array* list, unsigned int position)
{
  return list->array[position];
}

/** Libera todos los recursos asociados a la lista */
void Array_destroy(Array* list)
{
  for (size_t i = 0; i < list->count; i++) {
    // TODO: ADD close(client) and sending a message to the client so it closes aswell.
    free(&list->array[i]);
  }
  free(list->array);
  free(list);
}

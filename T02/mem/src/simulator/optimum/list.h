#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
  int* data;
  int size;
  int count;
} list;


typedef struct lists {
  list** data;
  int size;
  int count;
} lists;

list* list_init();

lists* lists_init();

void list_insert(list* l, int position, int elem);

int list_delete(list* l, int position);

list* list_copy(list* l); // crea un nuevo puntero a una lista igual a l

void lists_insert(lists* l, int position, list* elem);

void lists_add(lists* l1, lists* l2); // agrega todos los elementos de l2 a l1

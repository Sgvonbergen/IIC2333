#pragma once

typedef struct array_list {
  int* array;
  int size;
  int count;
} Array;

Array* Array_init();

void Array_append(Array* list, int element);

int Array_delete(Array* list, unsigned int position);

int Array_get(Array* list, unsigned int position);

void Array_destroy(Array* list);

#pragma once
#include "Client.h"

typedef struct array_list {
  Client* array;
  int size;
  int count;
} Array;

Array* Array_init();

void Array_append(Array* list, Client element);

Client Array_delete(Array* list, unsigned int position);

Client Array_get(Array* list, unsigned int position);

void Array_delete_by_socket_id(Array* list, int socket_id);

void Array_destroy(Array* list);

#include "list.h"
#include <stdlib.h>
#include <string.h>


void upgradeSize(list* l)
{
  l->size *= 2;
  l->data = realloc(l->data, sizeof(int) * l->size);
}

void s_upgradeSize(lists* l)
{
  l->size *= 2;
  l->data = realloc(l->data, sizeof(list*) * l->size);
}

list* list_init(){
  list* l = malloc(sizeof(list));
  l -> count = 0;
  l -> size = 8;
  l -> data = malloc(l->size * sizeof(int));
  return l;
}

void list_insert(list* l, int position, int element)
{
  if (l->count>=l->size) {
    upgradeSize(l);
  }
  for (int i = l->count-1; i >= position; i--) {
    l->data[i+1] = l->data[i];
  }
  l->data[position] = element;
  l->count += 1;
}

int list_delete(list* l, int position)
{
  int element = l->data[position];
  for (int i = position; i < l->count; i++) {
    l->data[i] = l->data[i+1];
  }
  l->count -= 1;
  return element;
}

list* list_copy(list* l){
  list* new = list_init();
  for (int i = 0; i < l->count ; i++){
    if (new->count >= new->size) {
      upgradeSize(new);
    }
    new->data[i] = l->data[i];
    new->count += 1;
  }
  return new;
}

lists* lists_init(){
  lists* l = malloc(sizeof(lists));
  l -> count = 0;
  l -> size = 4000;
  l -> data = malloc(l->size * sizeof(list*));
  return l;
}

void lists_insert(lists* l, int position, list* element)
{
  if (l->count>=l->size) {
    s_upgradeSize(l);
  }
  for (int i = l->count-1; i >= position; i--) {
    l->data[i+1] = l->data[i];
  }
  l->data[position] = element;
  l->count += 1;
}

void lists_add(lists* l1, lists* l2){ // agrega todos los elementos de l2 a l1

  int lenl1 = l1->count;
  for (int i = 0; i < l2->count; i++){
    lists_insert(l1, lenl1 + i, l2->data[i]);
  }
}

#include "Process.h"
#include <stdlib.h>
#include <string.h>

Process* p_init(int PID, char cmd[32], char** args, int arg_count)
{
  Process* p = malloc(sizeof(Process));
  p->PID = PID;
  p->arg_count = arg_count;
  strcpy(p->cmd, cmd);
  p->args = malloc((arg_count+1) * sizeof(char*));
  for (size_t i = 0; i < arg_count-1; i++) {
    p->args[i] = malloc(32*sizeof(char));
    strcpy(p->args[i],args[i]);
  }
  p->args[arg_count-1] = NULL;
  p->repeated = 0;
  p->code = 0;
  return p;
}

void upgradeSize(PList* list)
{
  list->size *= 2;
  list->array = realloc(list->array, sizeof(Process*) * list->size);
}

PList* p_list_init()
{
  PList* list = malloc(sizeof(PList));
  list->size = 8;
  list->count = 0;
  list->array = malloc(sizeof(Process*) * list->size);
  return list;
}

void p_list_append(PList* list, Process* element)
{
  if (list->count>=list->size) {
    upgradeSize(list);
  }
  list->array[list->count] = element;
  list->count += 1;
}

Process* p_list_get(PList* list, int position)
{
  return list->array[position];
}

int is_empty(PList* list)
{
  return (list->count <= 0);
}

Process* p_list_remove(PList* list, int position)
{
  if (position == list->count-1){
    return p_list_pop(list);
  }
  Process* p = p_list_get(list, position);
  for (size_t i = position+1; i < list->count; i++) {
    list->array[i-1] = list->array[i];
  }
  list->count--;
  return p;
}

Process* p_list_pop(PList* list)
{
  Process* p = p_list_get(list, list->count-1);
  list->count--;
  return p;
}

void p_list_destroy(PList* list)
{
  for (size_t i = 0; i < list->count; i++) {
    Process* p = p_list_get(list, i);
    for (size_t i = 0; i < p->arg_count; i++) {
      free(p->args[i]);
    }
    free(p->args);
    free(p);
  }
  free(list->array);
  free(list);
}

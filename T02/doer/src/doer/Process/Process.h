#pragma once
#include <unistd.h>
#include <stdio.h>

typedef struct process {
  int PID;
  int repeated;
  int code;
  char cmd[32];
  int arg_count;
  char** args;
  char output[BUFSIZ];
  int output_buffer;
} Process;

typedef struct p_list {
  Process** array;
  int count;
  int size;
} PList;

Process* p_init(pid_t PID, char cmd[32], char** args, int arg_count);

PList* p_list_init();

void p_list_append(PList* list, Process* element);

Process* p_list_get(PList* list, int position);

Process* p_list_pop(PList* list);

Process* p_list_remove(PList* list, int position);

int is_empty(PList* list);

void p_list_destroy(PList* list);

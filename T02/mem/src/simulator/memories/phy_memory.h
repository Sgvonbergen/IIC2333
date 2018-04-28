#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct frame {
  int time_stamp;
  int empty;
  int* entries;
} frame;

typedef struct physical_memory {
  frame** frames;
  int time;
} physical_memory;

frame* frame_init();

physical_memory* physical_memory_init();

int physical_memory_read(physical_memory* m, int frame_number, int offset);

int physical_memory_findfree(physical_memory* m);

int physical_memory_findreplace(physical_memory* m);

void physical_memory_replaceframe(physical_memory* m, int frame_number, int* new_entries);

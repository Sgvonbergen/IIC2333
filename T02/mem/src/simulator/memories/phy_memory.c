#include "phy_memory.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


frame* frame_init(){
    frame* f = malloc(sizeof(frame));
    f->empty = 1;
    f->entries = malloc(sizeof(unsigned int)*256);
    return f;
}

physical_memory* physical_memory_init(){
    physical_memory* m = malloc(sizeof(physical_memory));
    m->time = 0;
    m->frames = malloc(sizeof(frame*)*256);
    for (int i = 0; i < 256; i++){
      m->frames[i] = frame_init();
    }
    return m;
}

unsigned int physical_memory_read(physical_memory* m, int frame_number, int offset){
    frame* f = m->frames[frame_number];
    f->time_stamp = m->time;
    m->time += 1;
    return f->entries[offset];
}

int physical_memory_findfree(physical_memory* m){
    for (int i = 0; i < 256; i++){
      frame* f = m->frames[i];
      if (f->empty == 1){
        return i;
      }
    }
    return -1; //significa que no encontro ningun frame libre
}

int physical_memory_findreplace(physical_memory* m){
      int min = INFINITY;
      int index ;
      for (int i = 0; i < 256; i++){
        frame* f = m->frames[i];
        //printf("frame: %d time_stamp: %d\n", i, f->time_stamp);
        if (f->time_stamp < min){
          index = i;
          min = f->time_stamp;
        }
      }
      return index;
}

void physical_memory_replaceframe(physical_memory* m, int frame_number,unsigned int* new_entries, int page){
      frame* f = m->frames[frame_number];
      f->entries = new_entries;
      f->empty = 0;
      f->time_stamp = m->time;
      f->map = page; //esto nos permite invalidar el mapeo cuando se remplace por un nuevo page
      m->time += 1;
      return;
}

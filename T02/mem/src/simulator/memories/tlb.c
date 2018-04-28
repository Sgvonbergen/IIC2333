#include "tlb.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


void tlb_entry_update(tlb_entry* entry, int page_number, int frame_number, int t){
    entry->empty = 0;
    entry->time_stamp = t;
    entry->page_number = page_number;
    entry->frame_number = frame_number;
}

tlb* tlb_init(){
    tlb* t = malloc(sizeof(tlb));
    t->entries = malloc(sizeof(tlb_entry)*3);
    for (int i = 0; i < 3; i++){
      t->entries[i].empty = 1;
    }
    t->time = 0;
    return t;
}



tuple* tlb_lookup(tlb* t, int adress){
  for (int i = 0; i < 3; i++){
    tlb_entry entry = t->entries[i];
    if (entry.empty == 0){
      if (entry.page_number == adress){
        t->entries[i].time_stamp = t->time;
        t->time += 1;
        tuple* tpl = malloc(sizeof(tuple));
        tpl->succes = 1;
        tpl->entry = entry;
        return tpl;
      }
    }
  }
  tuple* tpl = malloc(sizeof(tuple));
  tpl->succes = 0;
  return tpl;
}



void tlb_insert(tlb* t, int adress, int frame_number){
    // busque si existe alguna entrada vacia y al mismo tiempo cual tiene el menor
    //time_stamp (en caso de no encontrar ninguna celda vacia)
    int min = INFINITY;
    int min_index;
    for (int i=0; i < 3; i++){
      tlb_entry entry = t->entries[i];
      if (entry.empty == 1){
          tlb_entry_update(&(t->entries[i]), adress, frame_number, t->time);
          t->time += 1;
          return;
      }
      if (entry.time_stamp < min){
        min = entry.time_stamp;
        min_index = i;
      }
    }
    //si llego hasta aqui significa que no encontre ninguna celda vacia
    tlb_entry_update(&(t->entries[min_index]), adress, frame_number, t->time);
    return;
}

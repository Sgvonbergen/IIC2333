#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


typedef struct tlb_entry {
  int page_number;
  int frame_number;
  int time_stamp;
  int empty;
} tlb_entry;


typedef struct tlb {
  tlb_entry* entries;
  int time; //para simular un time_stamp
} tlb;

typedef struct tuple{
  tlb_entry entry;
  int succes;
} tuple;

void tlb_entry_update(tlb_entry* entry, int page_number, int frame_number, int t);

tlb* tlb_init();

tuple* tlb_lookup(tlb* t, int adress);

void tlb_insert(tlb* t, int adrees, int frame_number);

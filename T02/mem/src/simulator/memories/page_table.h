#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../optimum/list.h"


typedef struct page_entry {
  int frame_number;
  int validity;  // es 1 si es valido y 0 si no es valido
} page_entry;

typedef struct page_table page_table;

struct page_table {
  page_table** sub_tables;  //esta lista se usa si no esta en el nivel mas alto
  page_entry** entries;  //si se encuentra en el nivel mas bajo
  int level; //nos ayuda a determinar en que nivel estamos
  int mask;  //podemos determinar el tamaño de las paginas
} ;


page_entry* page_entry_init();

page_table* page_table_init(list* levels_bits);

page_entry* page_table_read(page_table* table, int page);

void pag_table_update(page_table* table, int page, int frame, int validity);

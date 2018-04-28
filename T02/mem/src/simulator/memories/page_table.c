#include "page_table.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../optimum/list.h"

page_entry* page_entry_init(){
  page_entry* entry = malloc(sizeof(page_entry));
  entry->validity = 0;
  return entry;
}

page_table* page_table_init(list* levels_bits){
    page_table* pg = malloc(sizeof(page_table));
    pg->level = levels_bits->count;

    int bits = list_delete(levels_bits, 0);
    int size = pow(2,bits);

    pg->mask = 1;
    for (int k = 0; k < levels_bits->count; k++){
      pg->mask = pg->mask*pow(2, levels_bits->data[k]);
    }

    if (levels_bits->count == 0){  //caso en donde se llega al nivel mas bajo
        page_entry** entries = malloc(sizeof(page_entry*)*size);
        for(int i = 0; i < size; i++){
            page_entry* new_entry = page_entry_init();
            entries[i] = new_entry;
        }
        pg->entries = entries;
        return pg;
    }

    page_table** sub_tables = malloc(sizeof(page_table*)*size);
    for(int i = 0; i < size; i++){
      list* levels_bits_copy = list_copy(levels_bits);
      page_table* sub_table = page_table_init(levels_bits_copy);
      sub_tables[i] = sub_table;
    }
    pg->sub_tables = sub_tables;
    return pg;
}

page_entry* page_table_read(page_table* table, int page){

    //printf("en nivel: %d \n", table->level);
    if (table->level == 1){
      return table->entries[page];
    }
    int mask = table->mask;
    //printf("tengo mask: %d \n", mask);
    int index = page/mask;
    int sub_index = page%mask ;
    //printf("tengo index: %d sub index: %d\n", index, sub_index);
    page_table* next_table = table->sub_tables[index];
    return page_table_read(next_table, sub_index);
}


void page_table_update(page_table* table, int page, int frame, int validity){
    page_entry* entry =  page_table_read(table, page);
    entry->validity = validity;
    entry->frame_number = frame;
    return;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "optimum/list.h"
#include "optimum/optimum.h"
#include "memories/tlb.h"
#include "memories/page_table.h"
#include "memories/phy_memory.h"
#include <math.h>

unsigned int* get_page(int page_number){
  FILE* f;
  f = fopen("./data.bin", "r");
  fseek(f, page_number, SEEK_SET);
  unsigned int* buffer = malloc(sizeof(unsigned int)*256);
  fread(buffer, sizeof(unsigned int), 256, f);
  fclose(f);
  return buffer;
}

list* get_addresses(const char *filename){
  list* adresses = list_init();
  FILE* f = fopen(filename, "r" );
  char str[60];
  while (fgets(str,60,f)) {
    int adress = atoi(str);
    list_append(adresses, adress);
  }
  return adresses;
 }





int main(int argc, char const *argv[]) {

    //unsigned int* asd = get_page(0);
    //printf("asd: %u\n", asd[1] );
    int levels = atoi(argv[1]);


    physical_memory* memory = physical_memory_init();
    list* parameters = optimum(20, levels);
    list* parameters_copy = list_copy(parameters);
    for (int j = 0; j < parameters->count; j++){
      printf("BITS NIVEL %d: %d \n", j, parameters->data[j]);
    }
    printf("ESPACIO UTILIZADO: %lfB\n", score(parameters)*0.125);
    page_table* multilevel_table = page_table_init(parameters);
    tlb* tlb_table = tlb_init();

    list* adresses = get_addresses(argv[2]);

    double tlb_hits = 0;
    double tlb_miss = 0;
    double page_hits = 0;
    double page_faults = 0;

    for (int i = 0; i < adresses->count; i++){
      int logical_adress = adresses->data[i];
      printf("-%d-\n", logical_adress);
      int offset = logical_adress%256;
      int page_number = logical_adress/256;
      unsigned int content;
      tuple* tpl = tlb_lookup(tlb_table, page_number); //busco el frame en la tlb
      if (tpl->succes == 1){ //si el frame estaba en la tlb
          tlb_hits += 1;
          content = physical_memory_read(memory, tpl->entry.frame_number, offset);
          printf("Direccion Fisica: %d\n", tpl->entry.frame_number*256 + offset);
      }
      else{ //si el frame no estaba en la tlb
        tlb_miss += 1;
        page_entry* entry = page_table_read(multilevel_table, page_number); //busco el frame en la table multinivel
        if (entry->validity){ //si el frame esta en la tabla
            page_hits += 1;
            tlb_insert(tlb_table, page_number, entry->frame_number);
            content = physical_memory_read(memory, entry->frame_number, offset);
            printf("Direccion Fisica: %d\n", entry->frame_number*256 + offset);
        }
        else { //ocurre un page fault
            page_faults += 1;
            int free_frame_index = physical_memory_findfree(memory); //busco un espacio fisico para poner el nuevo frame
            if (free_frame_index == -1){ //caso en donde no queda espacio en la memoria fisica
              int free_frame_index = physical_memory_findreplace(memory);
              int previus_page = memory->frames[free_frame_index]->map; //necesito invalidar el mapeo anterior
              page_table_update(multilevel_table, previus_page, 0, 0);

            }
            //encuentro un espacio libre
            page_table_update(multilevel_table, page_number, free_frame_index, 1);
            tlb_insert(tlb_table, page_number, free_frame_index);
            unsigned int* new_entries = get_page(page_number);
            physical_memory_replaceframe(memory, free_frame_index, new_entries, page_number);
            content = physical_memory_read(memory, free_frame_index, offset);
            printf("Direccion Fisica: %d\n", free_frame_index*256 + offset);

        }
      }
      printf("contenido %u\n", content);
    }
    printf("PORCENTAJE_PAGE_FAULTS = %lf%% \n", 100*(page_faults/(page_faults+page_hits)));
    printf("PORCENTAJE_TLB_HITS = %lf%% \n", 100*(tlb_hits/(tlb_miss+tlb_hits)));

    printf("TLB\n" );
    printf("i\t" );
    for (int i = 0; i < parameters_copy->count; i++){
      printf("n%d_number\t", i+1);
    }
    /*
    printf("frame_number\n");
    for (int k = 0; k < 64; k++){
      printf("%d\t",k);
      int number = tlb_table->entries[k].page_number;
      for (int p = 0; p < parameters_copy->count; p++){
        printf("%d\t", number);
      }
      printf("%d\n", tlb_table->entries[k].frame_number );

    }
    */
  /*
  int* content = malloc(sizeof(int)*256);
  for (int i = 0; i < 256; i++){
    content[i] = i;
  }

  physical_memory* m = physical_memory_init();
  for (int j = 0; j < 256; j++){
    physical_memory_replaceframe(m, j, content);
  }
  int k = physical_memory_read(m, 0, 1);
  printf("%d\n", k);
  int p = physical_memory_findfree(m);
  printf("%d\n", p);
  int r = physical_memory_findreplace(m);
  printf("%d\n", r);
  */

  /*
  list* s = optimum(20, 5);
  printf("--\n" );
  for (int i=0; i< s->count; i++){
    printf("%d\n", s->data[i]);
  }

  page_table* table = page_table_init(s);

  page_table* t1 = table->sub_tables[0];
  page_table* t2 = t1->sub_tables[127];
  page_entry* p1 = t2->entries[63];
  p1->validity = 1;
  p1->frame_number = 100;
  printf("zxc: %d\n", p1->validity);
  printf("zxc: %d\n", p1->frame_number);

  page_table_update(table, 8191, 200, 0);

  page_entry* re = page_table_read(table, 8191);
  printf("asd: %d\n", re->validity);
  printf("asd: %d\n", re->frame_number);
  */

  /*
  list* s = optimum(20, 5);
  printf("--\n" );
  for (int i=0; i< s->count; i++){
    printf("%d\n", s->data[i]);
  }
  */

  /*
  //ejemplo visto en clases (http://iic2333.ing.puc.cl/slides/4-mem.html#/7/8)
  //funciona perfectamente
  tlb* table = tlb_init();
  printf("asd\n" );
  tuple* tpl = tlb_lookup(table, 0);
  printf("%d\n", tpl->succes );
  tlb_insert(table, 0 , 100);
  tuple* tpl1 = tlb_lookup(table, 1);
  printf("%d\n", tpl1->succes );
  tlb_insert(table, 1 , 200);
  tuple* tpl2 = tlb_lookup(table, 2);
  printf("%d\n", tpl2->succes );
  tlb_insert(table, 2 , 300);
  tuple* tpl3= tlb_lookup(table, 0);
  printf("s: %d\n", tpl3->succes );
  printf("d: %d\n", tpl3->entry.frame_number );
  tuple* tpl4= tlb_lookup(table, 1);
  printf("s: %d\n", tpl4->succes );
  printf("d: %d\n", tpl4->entry.frame_number );
  tuple* tpl5 = tlb_lookup(table, 3);
  printf("%d\n", tpl5->succes );
  tlb_insert(table, 3 , 400);
  tuple* tpl6= tlb_lookup(table, 0);
  printf("s: %d\n", tpl6->succes );
  tuple* tpl7= tlb_lookup(table, 3);
  printf("s: %d\n", tpl7->succes );
  printf("d: %d\n", tpl7->entry.frame_number );
  tuple* tpl8= tlb_lookup(table, 1);
  printf("s: %d\n", tpl8->succes );
  tuple* tpl9= tlb_lookup(table, 2);
  printf("s: %d\n", tpl9->succes );
  tlb_insert(table, 2 , 300);
  tuple* tpl10= tlb_lookup(table, 1);
  printf("s: %d\n", tpl10->succes );
  */

}

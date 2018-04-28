#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "optimum/list.h"
#include "optimum/optimum.h"
#include "memories/tlb.h"
#include "memories/page_table.h"
#include <math.h>

int main(int argc, char const *argv[]) {


  list* s = optimum(20, 3);
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

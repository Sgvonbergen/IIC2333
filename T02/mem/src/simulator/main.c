#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "optimum/list.h"
#include "optimum/optimum.h"
#include <math.h>

int main(int argc, char const *argv[]) {

  /*
  list* l1 = list_init();
  list_insert(l1,0,1);
  list_insert(l1,1,2);
  list* l2 = list_init();
  list_insert(l2,0,3);
  list_insert(l2,1,4);
  list* l3 = list_init();
  list_insert(l3,0,5);
  list_insert(l3,1,6);
  printf("%d\n", l1->data[0]);
  printf("%d\n", l1->data[1]);
  lists* ls1 = lists_init();
  lists* ls2 = lists_init();
  lists_insert(ls1, 0, l1);
  lists_insert(ls1, 1, l2);
  lists_insert(ls2, 0, l3);
  lists_add(ls1, ls2);


  list* l11 = ls1->data[2];
  printf("%d\n", l11->data[0]);
  printf("%d\n", l11->data[1]);
  */
  list* s = optimum(20, 5);
  printf("--\n" );
  for (int i=0; i< s->count; i++){
    printf("%d\n", s->data[i]);
  }

}

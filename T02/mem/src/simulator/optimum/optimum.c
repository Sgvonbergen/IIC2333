#include "list.h"
#include "optimum.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


lists* get_convinations(int len, int level){
  // caso base
  if (level == 1){
    lists* solutions = lists_init(); //el numero de combinaciones es lo maximo que puede a llegar ser
    list* l = list_init();
    list_insert(l,0,len);
    lists_insert(solutions, 0, l);
    return solutions;
  }
  // caso recursivo
  lists* total = lists_init();
  for (int i = 1; i < len - level + 2; i++){

      if (level == 3){
      }


      lists* sub_convinations = get_convinations(len-i, level-1);
      for (int j = 0; j < sub_convinations->count; j++){
        //printf("%d\n", j);
        list* li = sub_convinations->data[j];
        list_insert(li, 0, i);
      }
      lists_add(total, sub_convinations);
  }
  return total;
}


list* optimum(int len, int level){
    lists* convinations = get_convinations(len, level);
    int min = INFINITY;
    list* solution = list_init();
    for (int i = 0; i < convinations->count; i++){
      list* li = convinations->data[i];
      int s = score(li);
      if (s < min){
        min = s;
        solution = li;
      }
    }
    return solution;
}


int score(list* l){
  int score = 0;
  for (int i = 0; i < l->count; i++){
    if (i == l->count - 1){
      score += pow(2, l->data[i])*11;
      continue;
    }
    score += pow(2, l->data[i])* l->data[i+1];
  }
  return score;
}

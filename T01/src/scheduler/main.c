// Library for input output and file reading
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structures for managing processes and priority queues.

#include "queue/queue.h"
#include "process/process.h"


int main(int argc, char* argv[])
{
  // if version parameter exists
  if (argc >= 2) {
    if (strcmp(argv[1], "v1") == 0) {
      uint numeros[100];
      uint *bursts = numeros;
      bursts[0] = 1;
      process *p1 = process_init(1, "asd", 100, bursts, 10);

      Queue *q1 = Queue_init();
      Queue_append(q1, p1);
      process *p11 = Queue_get(q1, 0);
      uint *b = p11-> bursts;
      printf("%d \n", b[0] );


    }
  }
  return 0;
}

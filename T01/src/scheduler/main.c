// Library for input output and file reading
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structures for managing processes and priority queues.

#include "queue/queue.h"
#include "process/process.h"
#include "mlqf/mlqf.h"


int main(int argc, char* argv[])
{
  // if version parameter exists
  if (argc >= 2) {
    if (strcmp(argv[1], "v1") == 0) {
      uint numeros[100];
      uint *bursts = numeros;
      bursts[0] = 1;
      process *p1 = process_init(1, "primer proceso", 100, bursts, 10);
      process *p2 = process_init(1, "segundo proceso", 100, bursts, 10);
      process *p3 = process_init(1, "tercer proceso", 100, bursts, 10);


      mlqf* scheduler = mlqf_init(2, 3);
      mlqf_add_process(scheduler, p1);
      mlqf_add_process(scheduler, p2);

      Queue* q2 = scheduler->queues[1];
      Queue_append(q2, p3);

      process* p11 = mlqf_get_next_process(scheduler);
      printf("%s\n", p11->name);
      process* p21 = mlqf_get_next_process(scheduler);
      printf("%s\n", p21->name);
      process* p31 = mlqf_get_next_process(scheduler);
      printf("%s\n", p31->name);

    }
  }
  return 0;
}

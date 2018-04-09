// Library for input output and file reading
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structures for managing processes and priority queues.

#include "queue/queue.h"
#include "process/process.h"
#include "mlqf/mlqf.h"

Queue* fileReader(char* filename) {
  /* File reading */
  FILE* input;
  Queue* processes = Queue_init();
  char line[1024], *linePTR;
  input = fopen(filename, "r");
  char* name;
  // Error opening the file
  int i = 0;
  while (fgets(line, 512, input) != NULL) {
    linePTR = line;
    name = strsep(&linePTR, " ");
    uint t_init = atoi(strsep(&linePTR, " "));
    uint burst_amount = atoi(strsep(&linePTR, " "));
    uint* bursts = malloc(sizeof(int)*burst_amount);
    for (size_t i = 0; i < burst_amount; i++) {
      bursts[i] = atoi(strsep(&linePTR, " "));
    }
    Queue_append(processes, process_init(i, name, burst_amount, bursts, t_init));
  }
  return processes;
}


int main(int argc, char* argv[])
{
  if (argc >= 2) {
    if (strcmp(argv[1], "v1") == 0) {
      uint numeros1[1];
      uint numeros2[0];
      uint numeros3[0];
      uint *bursts1 = numeros1;
      uint *bursts2 = numeros2;
      uint *bursts3 = numeros3;
      bursts1[0] = 1;
      bursts1[1] = 2;
      bursts2[0] = 3;
      bursts3[0] = 1;
      process *p1 = process_init(1, "primer proceso", 2, bursts1, 10);
      process *p2 = process_init(1, "segundo proceso", 1, bursts2, 10);
      process *p3 = process_init(1, "tercer proceso", 1, bursts3, 10);


      mlqf* scheduler = mlqf_init(2, 2);
      mlqf_add_process(scheduler, p1);
      mlqf_add_process(scheduler, p2);
      mlqf_add_process(scheduler, p3);

      mlqf_tick(scheduler);
      mlqf_tick(scheduler);
      mlqf_tick(scheduler);
      mlqf_tick(scheduler);
      mlqf_tick(scheduler);
      mlqf_tick(scheduler);
      mlqf_tick(scheduler);
      mlqf_get_stats(scheduler, 7);

      //Queue* q1 = scheduler->queues[1];
      //printf("tiene: %d\n", q1->count);
      //unsigned int j = 0;
      //process* p12 = q1->array[j];
      //printf("%s\n", p12->name);
      //mlqf_update_processes(scheduler);


      //mlqf_tick(scheduler);
      //mlqf_tick(scheduler);

      /*
      Queue* q2 = scheduler->queues[1];
      Queue_append(q2, p3);

      process* p11 = mlqf_get_next_process(scheduler);
      printf("%s\n", p11->name);
      process* p21 = mlqf_get_next_process(scheduler);
      printf("%s\n", p21->name);
      process* p31 = mlqf_get_next_process(scheduler);
      printf("%s\n", p31->name);
      */

    }
  }
  return 0;
}

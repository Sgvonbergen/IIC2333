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

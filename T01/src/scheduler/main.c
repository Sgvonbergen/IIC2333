// Library for input output and file reading
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// Structures for managing processes and priority queues.
#include "queue/queue.h"
#include "process/process.h"
// Scheduler
#include "mlqf/mlqf.h"

// Global variables
mlqf* scheduler;
int t;

void signal_handler(int sig){
  if (sig == SIGINT){
    mlqf_get_stats(scheduler, t);
    mlqf_terminate(scheduler);
    exit(0);
  }
}

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
    Queue_append(processes, process_init(i, &name, burst_amount, bursts, t_init));
  }
  fclose(input);
  return processes;
}


int main(int argc, char* argv[])
{
  signal(SIGINT,signal_handler);
  if (argc >= 2) {
    if (strcmp(argv[1], "v1") == 0) {


      Queue* processes = fileReader(argv[2]);
      /*
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
      Queue* processes = Queue_init();
      process *p1 = process_init(1, "primer proceso", 2, bursts1, 0);
      process *p2 = process_init(1, "segundo proceso", 1, bursts2, 1);
      process *p3 = process_init(1, "tercer proceso", 1, bursts3, 2);
      Queue_append(processes, p1);
      Queue_append(processes, p2);
      Queue_append(processes, p3);
      */

      int quantum = atoi(argv[3]);
      int Q = atoi(argv[4]);

      int version;
      if (strcmp(argv[1], "v1")) {
        version = 1;
      } else if (strcmp(argv[1], "v12")) {
        version = 2;
        int s = atoi(argv[5]);
      } else if(strcmp(argv[1], "v3")){
        int s = atoi(argv[5]);
      }

      scheduler = mlqf_init(2, 2, 1);
      t = 1;
      int busy = 0;
      while(Queue_isempty(processes)==0 || busy ==1 ){
        process* p = Queue_get_process_by_start_time(processes, t-1);
        if (p != NULL){
          mlqf_add_process(scheduler, p);
        }
        busy = mlqf_tick(scheduler);
        t ++;
      }
      mlqf_get_stats(scheduler, t);


      /*
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
      */


    }
  }
  return 0;
}

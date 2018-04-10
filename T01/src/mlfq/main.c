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
    Queue* processes = fileReader(argv[2]);
    int quantum = atoi(argv[3]);
    int Q = atoi(argv[4]);
    int s = 0;
    int s_counter = 1;
    int version;
    if (strcmp(argv[1], "v1") == 0) {
      version = 1;
    } else if (strcmp(argv[1], "v2") == 0) {
      version = 2;
      s = atoi(argv[5]);
    } else if(strcmp(argv[1], "v3") == 0){
      version = 3;
      s = atoi(argv[5]);
    } else {
      return 1;
    }
    scheduler = mlqf_init(Q, quantum, version);
    t = 1;
    int busy = 0;
    while(Queue_isempty(processes)==0 || busy ==1 ){
      printf("T: %i\n", t);
      process* p = Queue_get_process_by_start_time(processes, t);
      if (p != NULL){
        mlqf_add_process(scheduler, p);
      }
      busy = mlqf_tick(scheduler);
      if (version == 2 || version == 3) {
        if (t == s*s_counter) {
          s_counter++;
          mlqf_resetqueues(scheduler);
        }
      }
      t++;
    }
    t--;
    mlqf_get_stats(scheduler, t);
    mlqf_terminate(scheduler);
    Queue_destroy(processes);
  } else {
    return 1;
  }
  return 0;
}

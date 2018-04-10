#include "process.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

process* process_init(uint PID, char** name, uint burstAmount, uint* bursts, uint start)
{
  process* p = malloc(sizeof(process));
  p->PID = PID;
  p->start = start;
  p->state = "READY";
  p->name = malloc(sizeof(char)*256);
  strcpy(p->name, *name);
  p->burstAmount = burstAmount;
  p->bursts = malloc(sizeof(uint)*burstAmount);
  for (size_t i = 0; i < burstAmount; i++) {
    p->bursts[i] = bursts[i];
  }
  free(bursts);
  p->burstLeft = p->bursts[0];
  p->cputimes = 0;
  p->readyTime = 0;
  p->turnarountime = 0;
  p->bloqueos = 0;
  p->responsetime = -1;
  p->currentBurst = 0;
  p->corresponding_queue = 0;
  p->time_running = 0;
  return p;
}

void process_tick(process* p)
{
  if (strcmp(p->state, "READY") == 0) {
    p->readyTime++;
  }
  if (strcmp(p->state, "RUNNING") == 0) {
    p->burstLeft--;
    /* turnarountime es el tiempo de ejecucion + tiempo de esperea. aqui sumo el tiempo de ejecucion
    y luego  cuando termina le agrego el tiempo que espero (readiTime)*/
    p->turnarountime++;
    p->time_running++;
  }
}


// retorno 0 si termino el burst actual y no quedan burst. si quedan burst retorno 1, si no ha terminado retorno 2
int process_check(process* p)
{
  printf("check of process '%s' \n", p->name );
  printf("currentBurst: %d\n", p->currentBurst);
  printf("burstAmount: %d\n", p->burstAmount -1 );
  printf("burst left: %d\n", p->burstLeft );
  if (p->burstLeft == 0) {
    p->time_running = 0;
    if (p->currentBurst == (p->burstAmount - 1)) {
      p->state = "FINISHED";
      p->turnarountime+= p->readyTime;
      printf("el proceso '%s' ha finalizado \n", p->name);
      return 0;
    } else {
      p->state = "READY";
      p->currentBurst++;
      p->burstLeft = p->bursts[p->currentBurst];
      return 1;
    }
  }
  else {
    return 2 ;
  }
  }

// retorna el tiempo que lleva corriendo en el burst actual
int process_time_running(process* p)
{
  return p->time_running;
}

void process_start(process* p)
{
  p->state = "RUNNING";
  if (p->responsetime < 0){
    p->responsetime = p->readyTime;
  }
}

void process_print(process* p)
{
  printf("%s:\n", p->name);
  printf("Turnos de CPU: %d\n", p->cputimes );
  printf("Bloqueos: %d\n", p->bloqueos);
  printf("Turnaround time: %d\n", p->turnarountime);
  printf("Response time: %d\n", p->responsetime );
  printf("Waiting time: %d\n", p->readyTime);
}

void process_destroy(process* p)
{
  free(p->bursts);
  free(p->name);
  free(p);
}

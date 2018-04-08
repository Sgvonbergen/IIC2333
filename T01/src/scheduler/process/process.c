#include "process.h"
#include <stdlib.h>
#include <string.h>

process* process_init(uint PID, char* name, uint burstAmount, uint* bursts, uint start)
{
  process* p = malloc(sizeof(process));
  p->PID = PID;
  p->start = start;
  p->state = "READY";
  p->name = name;
  p->burstAmount = burstAmount;
  p->bursts = malloc(sizeof(uint)*burstAmount);
  for (size_t i = 0; i < burstAmount; i++) {
    p->bursts[i] = bursts[i];
  }
  p->burstLeft = bursts[0];
  p->readyTime = 0;
  p->turnarountime = -1;
  p->responsetime = 0;
  p->currentBurst = 0;
  p->corresponding_queue = 0;
  return p;
}

void process_tick(process* p)
{
  if (strcmp(p->state, "READY") == 0) {
    p->readyTime++;
  }
  if (strcmp(p->state, "RUNNING") == 0) {
    p->burstLeft--;
    /* responsetime es el tiempo de ejecucion + tiempo de esperea. aqui sumo el tiempo de ejecucion
    y luego  cuando termina le agrego el tiempo que espero (readiTime)*/
    p->responsetime++;
  }
}


// retorno 0 si termino el burst actual y no quedan burst. si quedan burst retorno 1, si no ha terminado retorno 2
int process_check(process* p)
{
  if (p->burstLeft == 0) {
    if (p->currentBurst == (p->burstAmount - 1)) {
      p->state = "FINISHED";
      p->responsetime += p->readyTime;
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
  int t = p->bursts[p->currentBurst] - p->burstLeft;
  return t;
}

void process_start(process* p)
{
  p->state = "RUNING";
  if (p->responsetime < 0){
    p->responsetime = p->readyTime;
  }
}

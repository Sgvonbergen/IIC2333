#include "process.h"
#include <stdlib.h>
#include <string.h>

Process* Process_init(uint PID, char* name, uint burstAmount, uint* bursts, uint start)
{
  Process* p = malloc(sizeof(Process));
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
  p->currentBurst = 0;
  return p;
}

void Process_tick(Process* p)
{
  if (strcmp(p->state, "READY") == 0) {
    p->readyTime++;
  }
  if (strcmp(p->state, "RUNNING") == 0) {
    p->burstLeft--;
  }
}

void Process_int(Process* p)
{
  if (p->burstLeft == 0) {
    if (p->currentBurst == (p->burstAmount - 1)) {
      p->state = "FINISHED";
    } else {
      p->currentBurst++;
    }
  }
  p->state = "READY";
}

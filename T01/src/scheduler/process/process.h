//Estructura de process
#pragma once


struct process
{
  unsigned int PID;
  char* name;
  char* state;
  unsigned int start;
  unsigned int* bursts;
  unsigned int burstAmount;
  unsigned int currentBurst;
  unsigned int readyTime;
  unsigned int finishTime;
  unsigned int firstRun;
  unsigned int burstLeft;
};

typedef struct process Process;
typedef unsigned int uint;

Process* Process_init(uint PID, char* name, uint burstAmount, uint* bursts, uint start);

void Process_tick(Process* p);

void Process_int(Process* p);

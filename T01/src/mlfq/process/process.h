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
  unsigned int bloqueos;
  int responsetime;
  int turnarountime;
  unsigned int finishTime;
  unsigned int firstRun;
  unsigned int burstLeft;
  unsigned int corresponding_queue;
  unsigned int cputimes;
  unsigned int time_running;
  unsigned int original_queue;
};

typedef struct process process;
typedef unsigned int uint;

process* process_init(uint PID, char** name, uint burstAmount, uint* bursts, uint start);

void process_tick(process* p);

void process_print(process* p);

int process_check(process* p);

int process_time_running(process* p);

void process_start(process* p);

void process_destroy(process* p);

#pragma once
#include <unistd.h>

typedef struct process {
  pid_t PID;
  int repeated;
  char cmd[32];
  char** args;
} Process;

Process* p_init(pid_t PID, char cmd[32], char** args);

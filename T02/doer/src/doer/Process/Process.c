#include "Process.h"
#include <stdlib.h>
#include <string.h>

Process* p_init(pid_t PID, char cmd[32], char** args)
{
  Process* p = malloc(sizeof(Process));
  p->PID = PID;
  strcpy(p->cmd, cmd);
  p->args = args;
  p->repeated = 0;
  return p;
}

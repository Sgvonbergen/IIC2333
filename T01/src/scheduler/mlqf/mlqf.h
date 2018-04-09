#include "../queue/queue.h"
#include "../process/process.h"
#pragma once

struct mlqf
{
  Queue** queues;
  unsigned int Q;
  unsigned int quantum;
  unsigned int version;
  process* running_process;
  Queue* finished_processes;
};

typedef struct mlqf mlqf;

mlqf* mlqf_init(unsigned int Q, unsigned int quantum, unsigned int version);

int mlqf_tick(mlqf* m);

void mlqf_add_process(mlqf* m, process* p);

void mlqf_update_processes(mlqf* m);

void mlqf_get_stats(mlqf* m, int t);

process* mlqf_get_next_process(mlqf* m);

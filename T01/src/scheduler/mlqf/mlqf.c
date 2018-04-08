#include "mlqf.h"
#include "../process/process.h"
#include "../queue/queue.h"

// Libreria estandar de C
#include <stdlib.h>


mlqf* mlqf_init(unsigned int Q, unsigned int quantum)
{
  mlqf* m = malloc(sizeof(mlqf));
  m-> Q = Q;
  m-> quantum = quantum;
  m-> queues = malloc(sizeof(Queue)*Q);

  for (int i = 0; i < Q; i++){
    Queue* q = Queue_init();
    m->queues[i] = q;
  }

  return m;
}

void mlqf_add_process(mlqf* m, process* p)
  {
    Queue* q1 = m->queues[0];
    Queue_append(q1, p);
  }

void mlqf_tick(mlqf* m)
 {
   // primero actualizamos los procesos que estan en las colas
   mlqf_update_processes(m);

   // luego revisamos que sudece con el proceso que se esta corriendo en la cpu
   process* rp = m-> running_process;
   void process_tick(rp);
   int keep =  process_check(rp);
   // si keep es 0 significa que el burst esta terminado y no le quedan bursts
   if (keep == 0){
     rp->state = 'FINISHED';
     m->running_process = mlqf_get_next_process(m);
     process_start(m->running_process);
   }
   // si keep es 1 significa que el  burst esta terminado y aun le quedan bursts
   if (keep = 1){
     rp->state = 'READY';
     Queue* previus_queue = m->queues[rp->corresponding_queue];
     Queue_append(previus_queue, rp);
     m->running_process = mlqf_get_next_process(m);
     process_start(m->running_process);
   }
   // si keep es 2 significa que el burst todavia no a terminado
   if (keep = 2){
     int  time_runnig =  process_time_running(rp);
     if (time_runnig >= m->quantum){
       rp->state = 'READY';
       if (rp->corresponding_queue + 1 < m->Q){rp->corresponding_queue++;}; // supuesto: si me paso del quantum y estoy en el final de la cola, me quedo en la ultima cola
       Queue* next_queue = m->queues[rp->corresponding_queue];
       Queue_append(next_queue, rp);
       m->running_process = mlqf_get_next_process(m);
       process_start(m->running_process);

     }
   }

 }


process* mlqf_get_next_process(mlqf* m){
  for(int i = 0; i < m->Q ; i++ ){
    if (Queue_isempty(m->queues[i])) {
      continue;
    }
    else {
      process* p = Queue_pop(m->queues[i]);
      return p;
    }
  }
}


void mlqf_update_processes(mlqf* m){
  for(int i = 0; i < m->Q ; i++ ){
    Queue* qn = m->queues[i];
    for(int j = 0; j < qn->count; j++){
        process* pn = Queue_get(qn, j);
        process_tick(pn);
    }
   }
}

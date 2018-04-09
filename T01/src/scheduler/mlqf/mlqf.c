#include "mlqf.h"
#include "../process/process.h"
#include "../queue/queue.h"

// Libreria estandar de C
#include <stdlib.h>
#include <stdio.h>


mlqf* mlqf_init(unsigned int Q, unsigned int quantum, unsigned int version)
{
  mlqf* m = malloc(sizeof(mlqf));
  m-> Q = Q;
  m-> quantum = quantum;
  m-> queues = malloc(sizeof(Queue)*Q);
  m->finished_processes = Queue_init();
  //la lista de mayor prioridad estara en el espacio 0 el de menor prioridad en el espacio Q
  for (int i = 0; i < Q; i++){
    Queue* q = Queue_init();
    if (version == 3){  //si es la version 2 le asigno el quantum a cada cola
        unsigned int qi;
        qi = (i+1)*m->quantum;
        Queue_set_quantum(q, qi);
    }
    m->queues[i] = q;
  }
  m->running_process = NULL;
  return m;
}

void mlqf_add_process(mlqf* m, process* p)
  {
    Queue* q1 = m->queues[0]; //agrego el elemento a la lista de mayor priodad
    Queue_append(q1, p);
    printf("el proceso %s ha entrado al scheduler \n", p->name);
  }

int mlqf_tick(mlqf* m)
 {
   // primero revisamos se esta corriendo un proceso en la cpu

   process* rp = m-> running_process;
   if (rp == NULL){
     m->running_process = mlqf_get_next_process(m);
     rp = m->running_process;
     if (rp == NULL){
       return 0 ;        //si pido un proceso y no existe no tengo que hacer nada mas
     }
     else {
       process_start(rp);
     }
   }

   mlqf_update_processes(m); //actualizamos los valores de espera de los procesos en cola
   process_tick(rp);  //actualizamos el tiempo del proceso en la cpu

   int keep =  process_check(rp);
   // si keep es 0 significa que el burst esta terminado y no le quedan bursts
   if (keep == 0){
     rp->state = "FINISHED";
     printf("proceso %s ha terminado todos sus bursts\n", rp->name);
     Queue_append(m->finished_processes, rp);
     m->running_process = NULL;
     /*
     if (m->running_process == NULL){
       printf("el scheduller se ha quedado vacio\n");
       return 0;
     }
    process_start(m->running_process);
    */
   }
   // si keep es 1 significa que el  burst esta terminado y aun le quedan bursts
   if (keep == 1){
     rp->state = "READY";
     Queue* previus_queue = m->queues[rp->corresponding_queue];
     Queue_append(previus_queue, rp);
     printf("proceso %s ha terminado uno de sus bursts\n", rp->name );
     m->running_process = NULL;
     /*m->running_process = mlqf_get_next_process(m);
     process_start(m->running_process);*/
   }
   // si keep es 2 significa que el burst todavia no a terminado
   if (keep == 2){
     int  time_runnig =  process_time_running(rp);

     if (m->version == 3){ //para el caso de la version 2 seteo el quantum de la cola correspondiente
       Queue* q = m->queues[rp->corresponding_queue];
       m->quantum = q->q;
     }

     if (time_runnig >= m->quantum){
       rp->state = "READY";
       rp->bloqueos ++;
       if (rp->corresponding_queue + 1 < m->Q){rp->corresponding_queue++;}; // supuesto: si me paso del quantum y estoy en el final de la cola, me quedo en la ultima cola
       Queue* next_queue = m->queues[rp->corresponding_queue];
       Queue_append(next_queue, rp);
       printf("proceso %s ha terminado su quantum \n", rp->name);
       m->running_process = NULL;
       /*m->running_process = mlqf_get_next_process(m);
       process_start(m->running_process);*/

     }
   }
  return 1;
 }

// retorna el primer elemento que vea en la lista de mas alta prioridad
process* mlqf_get_next_process(mlqf* m){
  for(int i = 0; i < m->Q ; i++ ){
    if (Queue_isempty(m->queues[i])) {
      continue;
    }
    else {
      process* p = Queue_pop(m->queues[i]);
      p->cputimes ++;
      printf("el proceso %s ha entrado a la cpu\n", p->name);
      return p;
    }
  }
  return NULL;
}


void mlqf_update_processes(mlqf* m){

  for(int i=0; i<m->Q; i++){
    Queue* q = m->queues[i];
    for(unsigned int j=0; j< q->count; j++){
      process* p = q->array[j];
      process_tick(p);
    }
  }
}

void mlqf_get_stats(mlqf* m, int t){
  printf("Procesos terminados: %d\n", m->finished_processes->count);
  printf("Tiempo total: %d\n", t);
  printf("\n");
  if (m->running_process != NULL){
    process_print(m->running_process);
    printf("\n");
  }
  Queue* qf =  m->finished_processes;
  for(unsigned int j=0; j< qf->count; j++){
    process* p = qf->array[j];
    process_print(p);
    printf("\n");
    }
  for(int i=0; i<m->Q; i++){
    Queue* q = m->queues[i];
    for(unsigned int j=0; j< q->count; j++){
       process* p = q->array[j];
       process_print(p);
     }
   }
 }

 void mlqf_resetqueues(mlqf* m){
    printf("se han reiniciado las prioridades\n" );
    for(int i=1; i<m->Q; i++){
      Queue* q = m->queues[i];
      for(int j=0; j< q->count; j++){
         process* p = Queue_pop(q);
         Queue_append(m->queues[0], p);
         }
     }
  }

 void mlqf_terminate(mlqf* m)
 {
   Queue* q;
   process* p;
   for (size_t i = 0; i < m->Q; i++) {
     q = m->queues[i];
     for (size_t j = 0; j < q->count; j++) {
       p = q->array[j];
       process_destroy(p);
     }
     Queue_destroy(q);
   }
   free(m->queues);
   free(m);
 }

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int empty(struct queue_t *q) { return (q->size == 0); }

void enqueue(struct queue_t *q, struct pcb_t *proc) {
  /* TODO: put a new process to queue [q] */
  if (q->size == MAX_QUEUE_SIZE)
    return;
  else
    q->proc[q->size++] = proc;

}

struct pcb_t *dequeue(struct queue_t *q) {
  /* TODO: return a pcb whose priority is the highest
   * in the queue [q] and remember to remove it from q
   * */

  /* Check if the queue is empty */
  if (q->size <= 0)
    return NULL;
  /* Search for the highest priority process to be dequeued */
  int idx = 0;
  for (int i = 0; i < q->size; i++) {
    if (q->proc[i]->prio < q->proc[idx]->prio)
      idx = i;
  }
  struct pcb_t *res = q->proc[idx];
  
  /* Dequeue */
  for (int i = idx; i < q->size - 1; i++) {
    q->proc[i] = q->proc[i + 1];
  }
  q->size--;
  return res;
}

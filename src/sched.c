
#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t queue_lock;
// static pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
// static pthread_mutex_t slot_lock = PTHREAD_MUTEX_INITIALIZER;

#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
#endif

int queue_empty(void) {
#ifdef MLQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
    int i ;

	for (i = 0; i < MAX_PRIO; i ++)
		mlq_ready_queue[i].size = 0;
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED
/* 
 *  Stateful design for routine calling
 *  based on the priority and our MLQ policy
 *  We implement stateful here using transition technique
 *  State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */



struct pcb_t * get_mlq_proc(void) {
	/*TODO: get a process from PRIORITY [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
  static unsigned long curr_slot[MAX_PRIO]; // contains list of slots of each ready_queue 
  static unsigned long prio = 0;
  static int initialized = 0;
  static int flag = 1; // flag = 1 => all slots are = 0 and vice versa
  
  if(!initialized){
    for(int i = 0; i < MAX_PRIO; i++){
      curr_slot[i] = MAX_PRIO - i;
    }
    initialized = 1;
  }
	struct pcb_t * proc = NULL;
  
  pthread_mutex_lock(&queue_lock);

	// Check if the previous priority queues are non-empty
  if (prio > 0){
    for (unsigned long i = 0; i < prio; i++){
      if (!empty(&mlq_ready_queue[i]) && curr_slot[i] > 0){
        prio = i;
        break;
      }
    }
  }
  
  // Check if the current priority queue is empty
	while (empty(&mlq_ready_queue[prio])) {
		prio ++;  // Move to the next priority queue

		// If all priority queues have been checked, reset to the first priority
		if (prio >= MAX_PRIO)
    prio = 0;

		// If the first priority queue is also empty, break the loop
		if (prio == 0 && empty(&mlq_ready_queue[prio]))
			break; 
	}

	// Get a process from the current priority queue
	if (!empty(&mlq_ready_queue[prio])) {
		proc = dequeue(&mlq_ready_queue[prio]);
    curr_slot[prio] --; // Decrease the slot counter

	// If the current slot counter reaches the maximum slots for this priority, move to the next priority
  if(curr_slot[prio] == 0){
    prio ++;
  }
    
  // if all slots of ready_queue = 0 => reset
  for(int i = 0; i < MAX_PRIO; i++){
    if(curr_slot[i] != 0) flag = 0;
  }
  if(flag){
    for(int i = 0; i < MAX_PRIO; i++){
      curr_slot[i] = MAX_PRIO - i;
    }
    flag = 1;
  }
	}

	pthread_mutex_unlock(&queue_lock);
	return proc;

  ////////////////////////////////////////////
}

void put_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

struct pcb_t * get_proc(void) {
	return get_mlq_proc();
}

void put_proc(struct pcb_t * proc) {
	return put_mlq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
	return add_mlq_proc(proc);
}
#else
struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;
	/*TODO: get a process from [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
	pthread_mutex_lock(&queue_lock);
	// get the next process from the ready queue
	if(!empty(&ready_queue)){
    proc = dequeue(&ready_queue);
  }
	pthread_mutex_unlock(&queue_lock);

	return proc;
}

void put_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&run_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif


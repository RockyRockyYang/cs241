/**
 *scheduler Lab
 * CS 241 - Fall 2016
 */
#include "libpriqueue.h"
#include "libscheduler.h"

typedef struct _job_t job_t;
typedef struct _core_t core_t;
/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job
  queue elements.
*/
int can_preempted(job_t* new_job, job_t* curr_job);
void update_time(int time);


struct _job_t {
  int id;
  int priority;
  int arrival_time;
  int exec_time;    
  int remain_time;
  int start_time;
  int track_time;
  /* Add whatever other bookkeeping you need into this struct. */
};

struct _core_t {
  int free;   // boolean representing whether the core is in use or not
  job_t *job; // the job that the core is currently running, if any
};

priqueue_t pqueue;
core_t *cores;
scheme_t scheme;
float waiting_time = 0; 
float turnaround_time = 0; 
float response_time = 0; 
int num_of_jobs;
int num_of_cores; 

int comparer_fcfs(const void *a, const void *b) { 
    job_t* first = (job_t*)a;
    job_t* second = (job_t*)b;
    if(first->arrival_time < second->arrival_time){return -1;}
    else if(first->arrival_time == second->arrival_time){return 0;}
    else{return 1;}
}

int break_tie(const void *a, const void *b) { return comparer_fcfs(a, b); }

int comparer_ppri(const void *a, const void *b) {
  // Complete as is
  return comparer_pri(a, b);
}

int comparer_pri(const void *a, const void *b) { 
    job_t* first = (job_t*)a;
    job_t* second = (job_t*)b;
    if(first->priority < second->priority){return -1;}
    else if(first->priority > second->priority){return 1;}
    else{return break_tie(a, b);}
}

int comparer_psjf(const void *a, const void *b) { 
    // a is old and b is new
    job_t* first = (job_t*)a;
    job_t* second = (job_t*)b;
    if(first->remain_time < second->remain_time){return -1;}
    else if(first->remain_time > second->remain_time){return 1;}
    else{return break_tie(a,b);}
}

int comparer_rr(const void *a, const void *b) {
  // Casting to void to silence compiler warning
  (void)a;
  (void)b;
  // Picking 1 arbitarily.
  return 1;
}

int comparer_sjf(const void *a, const void *b) { 
    job_t* first = (job_t*)a;
    job_t* second = (job_t*)b;
    if(first->exec_time < second->exec_time){return -1;}
    else if(first->exec_time > second->exec_time){return 1;}
    else{return break_tie(a,b);} 
}

void scheduler_start_up(int numcores, scheme_t s) {
    scheme = s;
    cores = malloc(numcores*sizeof(core_t));
    num_of_cores = numcores;
    for(int i=0; i<numcores; i++){
        cores[i].free = 1;
        cores[i].job = NULL;
    }

  switch (s) {
  case FCFS:
    priqueue_init(&pqueue, comparer_fcfs);
    break;

  case PRI:
    priqueue_init(&pqueue, comparer_pri);
    break;

  case PPRI:
    priqueue_init(&pqueue, comparer_ppri);
    break;

  case PSJF:
    priqueue_init(&pqueue, comparer_psjf);
    break;

  case RR:
    priqueue_init(&pqueue, comparer_rr);
    break;

  case SJF:
    priqueue_init(&pqueue, comparer_sjf);
    break;

  default:
    printf("Did not recognize scheme\n");
    exit(1);
  }
}

int scheduler_new_job(int job_number, int time, int running_time, int priority) {
    job_t* new_job = malloc(sizeof(job_t));
    new_job->id = job_number;
    new_job->priority = priority;
    new_job->arrival_time = time;
    new_job->exec_time = running_time;    
    new_job->remain_time = running_time;
    new_job->start_time = -1;
    new_job->track_time = time;
    num_of_jobs ++;

    update_time(time);
    //looking for idle core
    for(int i=0; i< num_of_cores; i++){
        if(cores[i].free == 1){
            cores[i].job = new_job;
            cores[i].free = 0;
            new_job->start_time = time;
            return i;
        }
    }
    //looking for chance for preempt
    if(scheme == PPRI || scheme == PSJF){
        int preempt = -1;
        for(int i=0; i< num_of_cores; i++){
            if(can_preempted(new_job, cores[i].job)){
                if(preempt == -1 || can_preempted(cores[i].job, cores[preempt].job))
                    preempt = i;
            }
        }
        if(preempt != -1){
            job_t* left_job = cores[preempt].job;
            // finish part???

            //update core and put it in queue
            priqueue_offer(&pqueue, left_job);
            cores[preempt].job = new_job;
            new_job->start_time = time;
            return preempt;
        }
    }
    priqueue_offer(&pqueue, new_job);
    return -1;
}

//find whether a core can be preempted
int can_preempted(job_t* new_job, job_t* curr_job){
    if(scheme == PPRI){
        if(new_job->priority < curr_job->priority){
            return 1;
        }
    }else if(scheme == PSJF){
        if(new_job->remain_time < curr_job->remain_time){
            return 1;
        }
    }
    return 0;
}

// update remain time for every running job
void update_time(int time){
    for(int i=0; i< num_of_cores; i++){
        if(cores[i].free != 1){
            job_t* curr_job = cores[i].job;
            curr_job->remain_time -= (time - curr_job->track_time);
            curr_job->track_time = time;
        }
    }

}

int scheduler_job_finished(int core_id, int job_number, int time) {
    update_time(time);
    job_t* curr = cores[core_id].job;
    waiting_time += (time - curr->arrival_time) - curr->exec_time;
    turnaround_time += time - curr->arrival_time; 
    response_time += curr->start_time - curr->arrival_time;
    free(cores[core_id].job);
    cores[core_id].job = NULL;
    cores[core_id].free = 1;

    if(priqueue_peek(&pqueue)){
        job_t* new_work = priqueue_poll(&pqueue);
        cores[core_id].job = new_work;
        cores[core_id].free = 0;
        if(new_work->start_time == -1){
            new_work->start_time = time;
        }
        new_work->track_time = time;
        return new_work->id;
    }
    return -1;
}

int scheduler_quantum_expired(int core_id, int time) { 
    update_time(time);
    job_t* curr_job = cores[core_id].job;
    if(curr_job->remain_time <= 0){
        waiting_time += (time - curr_job->arrival_time) - curr_job->exec_time;
        turnaround_time += time - curr_job->arrival_time; 
        response_time += curr_job->start_time - curr_job->arrival_time;
        free(cores[core_id].job);
        cores[core_id].job = NULL;
        cores[core_id].free = 1;
    }else{
        priqueue_offer(&pqueue, curr_job);
        cores[core_id].job = NULL;
        cores[core_id].free = 1;
    }

    if(priqueue_peek(&pqueue)){
        job_t* new_work = priqueue_poll(&pqueue);
        cores[core_id].job = new_work;
        cores[core_id].free = 0;
        if(new_work->start_time == -1){
            new_work->start_time = time;
        }
        new_work->track_time = time;
        return new_work->id;
    }    
    return -1; 
}

float scheduler_average_waiting_time() { 
    return waiting_time/num_of_jobs;
}

float scheduler_average_turnaround_time() { 
    return turnaround_time/num_of_jobs; 
}

float scheduler_average_response_time() { 
    return response_time/num_of_jobs; 
}

void scheduler_clean_up() {
  /* Do more cleaning up here */
  priqueue_destroy(&pqueue);

}

void scheduler_show_queue() {
  // This function is left entirely to you! Totally optional.
}

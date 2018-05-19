/**
 * Luscious Locks Lab 
 * CS 241 - Fall 2016
 */
#include "barrier.h"

// The returns are just for errors if you want to check for them.
int barrier_destroy(barrier_t *barrier) {
	pthread_mutex_destroy(&(barrier->mtx));
	pthread_cond_destroy(&(barrier->cv));
	int error = 0;
	return error;
}

int barrier_init(barrier_t *barrier, unsigned int num_threads) {
	pthread_mutex_init(&(barrier->mtx), NULL);
  	pthread_cond_init(&(barrier->cv), NULL);
  	barrier->n_threads = num_threads;
  	barrier->count = 0;
  	barrier->times_used = 0;
	int error = 0;
	return error;
}

int barrier_wait(barrier_t *barrier) {
	pthread_mutex_lock(&(barrier->mtx)); 
	barrier->count ++;
	pthread_mutex_unlock(&(barrier->mtx));
	if(barrier->count != barrier->n_threads){
		pthread_mutex_lock(&(barrier->mtx));
		pthread_cond_wait(&(barrier->cv) , &(barrier->mtx));
	}
	if(barrier->count == barrier->n_threads){
		pthread_mutex_lock(&(barrier->mtx)); 
		barrier->count = 0;
		pthread_cond_broadcast(&(barrier->cv));
	}
	pthread_mutex_unlock(&(barrier->mtx)); 
	return 0; 
}

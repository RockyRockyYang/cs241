/**
 * Luscious Locks Lab 
 * CS 241 - Fall 2016
 */
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Struct representing a node in a queue_t
 */
typedef struct queue_node_t {

  struct queue_node_t *next;
  void *data;
} queue_node_t;

/**
 * Struct representing a queue
 */
struct queue_t {

  queue_node_t *head, *tail;
  int size;
  int maxSize;
  pthread_cond_t cv;
  pthread_mutex_t m;
};

/**
 *  Given data, place it on the queue.  Can be called by multiple threads.
 *  Blocks if the queue is full.
 */
void queue_push(queue_t *queue, void *data) {
    /* Your code here */ 
    pthread_mutex_lock(&(queue->m));
    while(queue->maxSize >0 && queue->size == queue->maxSize){
        pthread_cond_wait(&(queue->cv), &(queue->m));
    }
        queue_node_t* node = malloc(sizeof(queue_node_t));
        node->data = data;
        if(queue->tail == NULL){ //no nodes
            queue->tail = node;
            queue->head = node;
            node->next = NULL;
        }else{
            queue->tail->next = node;
            node->next = NULL;
            queue->tail = node;
        }
        queue->size ++;
        pthread_cond_signal(&(queue->cv));

    pthread_mutex_unlock(&(queue->m));
}

/**
 *  Retrieve the data from the front of the queue.  Can be called by multiple
 * threads.
 *  Blocks if the queue is empty.
 */
void *queue_pull(queue_t *queue) {
  /* Your code here */
    void* data = NULL;
    pthread_mutex_lock(&(queue->m));
    while(queue->size == 0){
        pthread_cond_wait(&(queue->cv), &(queue->m));
    }
    queue_node_t* node = queue->head;
    data = node->data;
    if(queue->head == queue->tail){
        queue->head = NULL;
		queue->tail = NULL;
 	}else{
		queue->head = queue->head->next;
	}
	free(node);
    queue->size --;
    pthread_cond_signal(&(queue->cv));
    pthread_mutex_unlock(&(queue->m));
    return data;
}

/**
 *  Allocates heap memory for a queue_t and initializes it.
 *  Returns a pointer to this allocated space.
 */
queue_t *queue_create(int maxSize) {
  /* Your code here */
    queue_t* queue = malloc(sizeof(queue_t));
    if(maxSize < 0){ queue->maxSize = -1;}
    else{queue->maxSize = maxSize;}
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&(queue->m), NULL);
    pthread_cond_init(&(queue->cv), NULL);
    return queue;
}

/**
 *  Destroys the queue, freeing any remaining nodes in it.
 */
void queue_destroy(queue_t *queue) { 
/* Your code here */ 
    queue_node_t *p = queue->head;
    while(p != NULL){
        queue_node_t *temp = p->next;
        //free(p->data);
        free(p);
        p = temp;
    }
    pthread_mutex_destroy(&(queue->m));
    pthread_cond_destroy(&(queue->cv));
	free(queue);
}

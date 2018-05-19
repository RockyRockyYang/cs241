/*
 *machine Problem: Wearables
 * CS 241 - Fall 2016
 */

#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"
#include "vector.h"
#include "wearable.h"

int wearable_server_fd;
int request_server_fd;

Vector* timestamp_vector;
int* socket_id;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_t request_thread;
timestamp_entry* stamp;

typedef struct {
  pthread_t thread;
  int fd;
  long timestamp;
  int open;
} thread_data;

thread_data **wearable_threads;
int wearable_threads_size = 0;

bool select_beat(timestamp_entry *arg) {
  SampleData *data = (SampleData *)arg->data;
  if(strcmp(data->type_, TYPE1) == 0){return true;}
	else{return false;}
}

bool select_suger(timestamp_entry *arg) {
  SampleData *data = (SampleData *)arg->data;
  if(strcmp(data->type_, TYPE2) == 0){return true;}
	else{return false;}
}

bool select_temp(timestamp_entry *arg) {
  SampleData *data = (SampleData *)arg->data;
  if(strcmp(data->type_, TYPE3) == 0){return true;}
	else{return false;}
}

void signal_received(int sig) {
  // TODO close server socket, free anything you don't free in main
  if(sig==SIGINT) {
    close(wearable_server_fd);
    for(int i=0; i<wearable_threads_size; i++) {
      pthread_join(wearable_threads[i]->thread, NULL);  
      free(wearable_threads[i]);
			wearable_threads[i] = NULL;
      timestamp_entry* times = Vector_get(timestamp_vector, i);
      free(times->data);
			times->data = NULL;
    }
    pthread_join(request_thread, NULL);
    free(wearable_threads);
    free(socket_id);
    Vector_destroy(timestamp_vector);
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&cv);
    exit(0);
  }
}

void *wearable_processor_thread(void *args) {
  thread_data *td = (thread_data *)args;
  int socketfd = td->fd;
  char buffer[64];
  long timestamp = 0;
  while(read(socketfd, buffer, 64)>0) {
    timestamp_entry* curr = malloc(sizeof(timestamp_entry));
    SampleData* data;
    extract_key(buffer, &timestamp, &data);
    td->timestamp = timestamp;
    curr->time = timestamp;
    curr->data = data;
    pthread_mutex_lock(&m);
    stamp->time = timestamp;
    Vector_append(timestamp_vector, curr);
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&m);
  }
  td->open = 0;
  close(socketfd);
  pthread_mutex_lock(&m);
  pthread_cond_broadcast(&cv);
  pthread_mutex_unlock(&m);
  return NULL;
}

void *user_request_thread(void *args) {
  int socketfd = *((int *)args);
  // TODO read data from the socket
  // Requests will be in the form "<timestamp1>:<timestamp2>"
  // Write out statistics for data between those timestamp ranges
  char buffer[64];
  long start = 0;
  long end = 0;
  while(read(socketfd, buffer, 64)>0) {
    sscanf(buffer, "%li:%li", &start, &end);
    pthread_mutex_lock(&m);
    for(int i=0;i<wearable_threads_size; i++) {
      while(wearable_threads[i]->open==1 && wearable_threads[i]->timestamp<end) {
        pthread_cond_wait(&cv, &m);
      }
    }
    pthread_mutex_unlock(&m);
    timestamp_entry* output1 = NULL;
    timestamp_entry* output2 = NULL;
    timestamp_entry* output3 = NULL;
    ssize_t num_beat = gather_timestamps(timestamp_vector, start, end, select_beat, &output1);
    ssize_t num_suger = gather_timestamps(timestamp_vector, start, end, select_suger, &output2);
    ssize_t num_temp  = gather_timestamps(timestamp_vector, start, end, select_temp, &output3);
    write_results(socketfd, TYPE1, output1, num_beat);
    write_results(socketfd, TYPE2, output2, num_suger);
    write_results(socketfd, TYPE3, output3, num_temp);
    write(socketfd, "\r\n", 2);
    free(output1);
		output1 = NULL;
    free(output2);
		output2 = NULL;
    free(output3);
		output3 = NULL;
  }
  close(socketfd);
  return NULL;
}

int open_server_socket(const char *port) {
  // TODO given a string with the port value, set up a passive socket file
  // descriptor and return it
  int socket_num = socket(AF_INET, SOCK_STREAM, 0);
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  int optval = 1;
  setsockopt(socket_num, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
  int s = getaddrinfo(NULL, port, &hints, &result);
  if(s!=0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }
  if(bind(socket_num, result->ai_addr, result->ai_addrlen)!=0) {
    perror("bind fail");
    exit(1);
  }
  if(listen(socket_num, 20)!=0) {
    perror("listen fail");
    exit(1);
  }
  freeaddrinfo(result);
  return socket_num;
}

int wearable_server(const char *wearable_port, const char *request_port) {
  signal(SIGINT, signal_received);
  request_server_fd = open_server_socket(request_port);
  wearable_server_fd = open_server_socket(wearable_port);
  stamp = malloc(sizeof(timestamp_entry));
	wearable_threads = malloc(100*sizeof(thread_data*));
  socket_id = malloc(sizeof(int)*100);
  int request_socket = accept(request_server_fd, NULL, NULL);
  pthread_create(&request_thread, NULL, user_request_thread, &request_socket);
  close(request_server_fd);
  timestamp_vector = Vector_create(timestamp_entry_copy_constructor, timestamp_entry_destructor);
  while(1) {  
    if((socket_id[wearable_threads_size]=accept(wearable_server_fd, NULL, NULL))==-1) {
      perror("accept fail");
      exit(1);
    }
    wearable_threads[wearable_threads_size] = malloc(sizeof(thread_data));
    wearable_threads[wearable_threads_size]->fd = socket_id[wearable_threads_size];
    wearable_threads[wearable_threads_size]->open = 1;
    pthread_create(&wearable_threads[wearable_threads_size]->thread, NULL, wearable_processor_thread, wearable_threads[wearable_threads_size]);
    pthread_mutex_lock(&m);
    wearable_threads_size++;
    pthread_mutex_unlock(&m);
  }
  return 0;
}

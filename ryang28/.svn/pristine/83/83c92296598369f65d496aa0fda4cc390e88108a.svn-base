/**
 * Parallel Map Lab
 * CS 241 - Fall 2016
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "map.h"
#include "mappers.h"

/* You should create a struct that will get passed in by reference to your
 * start_routine. */

/* You should create a start routine for your threads. */
double *ret_list;
mapper *map_function;
typedef struct _sublist{
  double *list;
  size_t start;
  size_t end;
} sublist;

void* maplist(void* currlist){
	sublist* curr = currlist;
	mapper funct = *map_function;
	size_t i = curr->start;
	for(; i <= curr->end; i++){
    	ret_list[i] = funct((curr->list)[i]);
  	}
	return NULL;
}

double *par_map(double *list, size_t list_len, mapper map_func,
                size_t num_threads) {
  /* Your implementation goes here */
	map_function = &map_func;
	ret_list = (double*)malloc(sizeof(double) * list_len);
	if(list_len < num_threads){
		num_threads = list_len;	
	}
	pthread_t threads[num_threads];
	size_t sublength_1 = list_len / num_threads;;
	int i= 0;
	for(; i< (int)num_threads; i++){
		sublist* temp = (sublist*)malloc(sizeof(sublist));
		temp->start = i * sublength_1;
		if(i == (int)num_threads-1){
			temp->end = list_len -1;
		}else{ 
			temp->end = (i+1) * sublength_1 -1;
		}
		temp->list = list;
		pthread_create(&threads[i], NULL, maplist, (void*)temp);
	}

	for(int i=0; i< (int)num_threads;i++){
		void** retval = NULL;
		pthread_join(threads[i], retval);
	}
	return ret_list;
}


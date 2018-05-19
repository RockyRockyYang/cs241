/**
 * Machine Problem: Password Cracker
 * CS 241 - Fall 2016
 */

#include "cracker2.h"
#include "format.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <crypt.h>

char **strsplit(const char *str, const char *delim, size_t *numtokens);
void* password_cracker(void* elem);

typedef struct _input{
  char* username;
  char* hash;
  char* hint;
  int thread_id;
} input;

typedef struct _ret{
  char* password;
  int hashCount;
} ret;

size_t count_thread;
int success = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; 

void* password_cracker(void* elem){
	input* in = (input*)elem;
	int known_num = getPrefixLength(in->hint);
	int dot_num = strlen(in->hint) - known_num;
	int unknown_letter_count = dot_num;
	long start_index = 0;
	long count = 0;
	getSubrange(unknown_letter_count, count_thread, in->thread_id,
                 &start_index, &count);
	char* letter = strdup(in->hint);
	char* result = letter + known_num;
	setStringPosition(result, start_index);
	v2_print_thread_start(in->thread_id, in->username, start_index,
                           letter);
	// start find password...
	char* hashed = NULL;
	int hashCount = 0;
	struct crypt_data cdata;
	cdata.initialized = 0;
	// set up flags
	int found = 4;
	while(1){
		hashed = crypt_r(letter, "xx", &cdata);
		hashCount += 1;
		if(!strcmp(hashed, in->hash)){
			found = 0;
			pthread_mutex_lock(&m);
			success = 1;
			v2_print_thread_result(in->thread_id, hashCount, found);
			pthread_mutex_unlock(&m);
			break;
		}
		incrementString(result);
		if(count == hashCount){
			found = 2;
			v2_print_thread_result(in->thread_id, hashCount, found);
			break;			
		}
		pthread_mutex_lock(&m);
		if(success == 1){ // other thread find password
			found = 1;
			v2_print_thread_result(in->thread_id, hashCount, found);
			pthread_mutex_unlock(&m);
			break;
		}
		pthread_mutex_unlock(&m);
		
	}
	//v2_print_thread_result(in->thread_id, hashCount, found);
	//set up return value here
	ret* unit = malloc(sizeof(ret));
	unit->hashCount = hashCount;
	unit->password = NULL;
	if(found == 0){
		unit->password = strdup(letter);
	}
// free before return
	free(letter);
	letter = NULL;
	free(elem);
	elem = NULL;
	return (void*)unit;
}


int start(size_t thread_count) {
  // TODO your code here, make sure to use thread_count!
	double start_time = getTime();
  	double start_cpu_time = getCPUTime();
	count_thread = thread_count;
  	pthread_t threads[thread_count];
	char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, stdin)) != -1) {
		start_time = getTime();
	  	start_cpu_time = getCPUTime();
    	if(line[strlen(line)-1] == '\n')
    		line[strlen(line)-1] = '\0';
    	// split string in to pieces
    	size_t numtokens = 0;
    	char** command = strsplit(line, " ", &numtokens);
    	v2_print_start_user(command[0]);
    	// assign to each thread input
    	for(int i=0; i< (int)thread_count; i++){
    		input* elem = malloc(sizeof(input));
    		elem->username = command[0];
    		elem->hash = command[1];
    		elem->hint = command[2];
    		elem->thread_id = i+1;
			pthread_create(&threads[i], NULL, password_cracker, (void*)elem);
		}
		char *password = NULL;
		int hashCount = 0;
		int result = 1;
		for(int i=0; i< (int)thread_count; i++){
    		void* retval = NULL;
			pthread_join(threads[i], &retval);
			ret* ret_unit = (ret*)retval;
			hashCount += ret_unit->hashCount;
			if(ret_unit->password != NULL){
				password = strdup(ret_unit->password);
				result = 0;
				free(ret_unit->password);
				ret_unit->password = NULL;
			}
			free(retval);
			retval = NULL;
    	}
    	double elapsed = getTime() - start_time;
  		double total_cpu_time = getCPUTime() - start_cpu_time;
  		v2_print_summary(command[0], password, hashCount,
                      elapsed, total_cpu_time, result);

    	free(command[0]);
    	command[0] = NULL;
		free(command[1]);
		command[1] = NULL;
		free(command[2]);
		command[2] = NULL;
		free(command);
		command = NULL;
		free(password);
		password = NULL;
		// reset global variable and line buffer		
		success = 0;
		free(line);
		line = NULL;
    }
	free(line);
	line = NULL;
	pthread_mutex_destroy(&m);
	return 0;
}

char **strsplit(const char *str, const char *delim, size_t *numtokens) {
  char *s = strdup(str);
  size_t tokens_alloc = 1;
  size_t tokens_used = 0;
  char **tokens = calloc(tokens_alloc, sizeof(char *));
  char *token, *rest = s;
  while ((token = strsep(&rest, delim)) != NULL) {
    if (tokens_used == tokens_alloc) {
      tokens_alloc *= 2;
      tokens = realloc(tokens, tokens_alloc * sizeof(char *));
    }
    tokens[tokens_used++] = strdup(token);
  }
  if (tokens_used == 0) {
    free(tokens);
    tokens = NULL;
  } else {
    tokens = realloc(tokens, tokens_used * sizeof(char *));
  }
  *numtokens = tokens_used;
  free(s);
  return tokens;
}

/*machine Problem: Password Cracker
 * CS 241 - Fall 2016
 */
#include "cracker1.h"
#include "format.h"
#include "utils.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <crypt.h>

char **strsplit(const char *str, const char *delim, size_t *numtokens);
void* password_cracker(void* idx);
// no need to implement struct
queue_t* task_queue = NULL;

void* password_cracker(void* idx){
	int threadId = *(int*)idx;
	int sum_success = 0;
	while(1){
		char* task = queue_pull(task_queue);
		if(!strcmp(task, "end")){
			queue_push(task_queue, task);
			break;
		}
		size_t numtokens = 0;
		char** command = strsplit(task, " ", &numtokens);
		char* username = command[0];
		char* hash = command[1];
		//char* known = command[2]; 
		v1_print_thread_start(threadId, username); //threadid???
		// decrypt password
		char* letter = strdup(command[2]);
		int known_num = getPrefixLength(command[2]);
		char* result = letter + known_num;
		setStringPosition(result, 0);
		int unsuccess = 1;
		int hashCount = 0;
		char* hashed = NULL;
		struct crypt_data cdata;
		cdata.initialized = 0;
		double start = getThreadCPUTime();
		while(1){
			hashed = crypt_r(letter, "xx", &cdata);
			hashCount += 1;
			if(!strcmp(hashed, hash)){
				unsuccess = 0;
				break;
			}
			int ret = incrementString(result);
			if(!ret){
				unsuccess = 1;
				break;			
			}
			/*if(hashCount == stop){
				unsuccess = 1;
				//printf("%s\n", letter);
				break;
			}*/
		}
		double end = getThreadCPUTime();
		double timeElapsed = end - start;
		v1_print_thread_result(threadId, username, letter,
                          	hashCount, timeElapsed, unsuccess);
		if(!unsuccess){sum_success++;}
		//check for memory(strdup, strsplit...)
		free(task);		
		free(letter);
		free(command[0]);
		free(command[1]);
		free(command[2]);
		free(command);
	}
	// anymore free?
	return (void*)(long)sum_success;
}

int start(size_t thread_count) {
  // TODO your code here, make sure to use thread_count!
	pthread_t threads[thread_count];
	task_queue = queue_create(-1);
	int queue_size = 0;
	char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, stdin)) != -1) {
    	if(line[strlen(line)-1] == '\n')
    		line[strlen(line)-1] = '\0';
    	queue_push(task_queue, line);
		line = NULL;
    	queue_size++;
    }
	free(line);
	line = NULL;
    // create end signal
    char* end_signal = "end";
    queue_push(task_queue, end_signal);
    for(int i=0; i< (int)thread_count; i++){
		pthread_create(&threads[i], NULL, password_cracker, (void*)&i);
	}
	int numRecovered = 0;
    for(int i=0; i< (int)thread_count; i++){
    	void* retval = NULL;
		pthread_join(threads[i], &retval);
		numRecovered += (int)(long)retval;
    }
    v1_print_summary(numRecovered, queue_size - numRecovered);
    queue_destroy(task_queue);
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

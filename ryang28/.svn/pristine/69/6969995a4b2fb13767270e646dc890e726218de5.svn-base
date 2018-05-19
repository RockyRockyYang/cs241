/**
 * Parallel Make
 * CS 241 - Fall 2016
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include "parmake.h"
#include "common_vector.h"
#include "queue.h"
#include "rule.h"
#include "parser.h"

#define MAX_SIZE 32

queue_t* rules;
queue_t* rules_ready;
int rules_num = 0;
int rules_ready_size = 0;
int c = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int run_command(Vector *commands){
	int total = (int)Vector_size(commands);
	for(int i=0; i<total; i++){
		char* temp = Vector_get(commands, i);
		int ret = system(temp);
		if(ret != 0){
			return 1;
		}
	}
	return 0;
}


void* execute(void* ptr){
	pthread_mutex_lock(&lock);
	if(c >= rules_ready_size){
		pthread_mutex_unlock(&lock);
		return NULL;	
	}
	pthread_mutex_unlock(&lock);
	while(c < rules_ready_size){
	//printf("c is:%d\n", c);
		rule_t* curr;
		Vector* curr_dep;
		int standby = 1;
		//pthread_mutex_unlock(&lock);
		while(standby){
			pthread_mutex_lock(&lock);
			if(c >= rules_ready_size){
				pthread_mutex_unlock(&lock);
				return NULL;	
			}
			curr = queue_pull(rules_ready);
			c++;
			pthread_mutex_unlock(&lock);
			curr_dep = curr->dependencies;
			if(Vector_size(curr->dependencies) == 0){
				standby = 0;
				break;
			}else{
				int tmp_flag = 0;
				for(int i=0; i<(int)Vector_size(curr_dep); i++){
					int state_status = ((rule_t*)Vector_get(curr_dep, i))->state;
					if(state_status != -1 && state_status != 10){
						tmp_flag = 1;
						pthread_mutex_lock(&lock);
						queue_push(rules_ready, curr);
						c--;
						pthread_mutex_unlock(&lock);
						break;		
					}
				}
				if(tmp_flag == 0){
					standby = 0;
					break;
				}
			}
		}
		
		int run = 0;
		int fail = 0;
		// if has dependent
		if(Vector_size(curr->dependencies) != 0){
			for(int i=0; i< (int)Vector_size(curr_dep); i++){
				if(((rule_t*)Vector_get(curr_dep, i))->state == -1){
					fail = 1;
					curr->state = -1;
					break;
				}
			}	
		}
		if(fail == 1){
			continue;
		}
		int error = access(curr->target, F_OK);
		if(error == -1){
			run = 1;
			fail = run_command(curr->commands);
		}
		if(run == 0){
			for(int i=0; i< (int)Vector_size(curr_dep); i++){
				rule_t* temp = (rule_t*)Vector_get(curr_dep, i);
				int flag = access(temp->target, F_OK);
				if(flag == -1){
					run = 1;
					fail = run_command(curr->commands);
					break;
				}
			}
		}
		if(run == 0){
			struct stat s_1;
			struct stat s_2;
	   		stat(curr->target, & s_1);
	   		time_t curr_mod = s_1.st_mtime;
	   		for(int i=0; i< (int)Vector_size(curr_dep); i++){
	   			rule_t* temp = (rule_t*)Vector_get(curr_dep, i);
	   			stat(temp->target, & s_2);
	   			time_t temp_mod = s_2.st_mtime;
	   			if(difftime(temp_mod, curr_mod)>0){ //newer time
	   				run = 1;
	   				fail = run_command(curr->commands);
					break;
	   			}
	   		}
		}
		if(run == 0 || fail == 0){ //10 is satisfied
			curr->state = 10;
		}
		if(fail == 1){
			curr->state = -1;
		}
	}
	return NULL;
}

void parsed_new_target(rule_t *target){
	queue_push(rules, target);
	rules_num++;
}

Vector* sort_rules(){
	Vector* sorted_rules = Vector_create(copy_pointer, destroy_pointer);
	int count = 0;
	while(count < rules_num){
		rule_t* elem = queue_pull(rules);
		Vector* elem_dep = elem->dependencies;
		if(Vector_size(elem_dep) == 0 && elem->state != 1){
			size_t index = Vector_size(sorted_rules);
			Vector_insert(sorted_rules, index, elem);
			elem->state = 1;
			count = 0;
			for(int i=0; i<rules_num-1; i++){
				rule_t* curr = queue_pull(rules);
				Vector* curr_dep = curr->dependencies;
				for(int j=0; j<(int)Vector_size(curr_dep); j++){
					if(strcmp(elem->target, ((rule_t*)Vector_get(curr_dep, j))->target)==0){
						((rule_t*)Vector_get(curr_dep, j))->state = 1;
					}
				}
				queue_push(rules, curr);
			}
		}else if(elem->state != 1){
			int fail = 0;
			for(int i=0; i<(int)Vector_size(elem_dep);i++){
				if(((rule_t*)Vector_get(elem_dep, i))->state != 1){
					fail = 1;
					break;
				}
			}
			if(fail == 0){
				size_t idx = Vector_size(sorted_rules);
				Vector_insert(sorted_rules, idx, elem);
				elem->state = 1;
				count = 0;
			}
		}
		queue_push(rules, elem);
		count++;
	}
	return sorted_rules;
}

// Treat this as main
int parmake(int argc, char **argv) {
	int opt;
	//int f_flag, j_flag = 0;
	int thread_num = 1; 
	char* filename = NULL;
	char** targets = NULL;

	while((opt = getopt(argc, argv, "f:j:")) != -1){
		switch(opt){
			case 'f':
				filename = optarg;
        		break;
			case 'j':
				if(atoi(optarg)!= 0){
					thread_num = atoi(optarg);
				}
				break;
			case '?':
				perror("input error!");
				return 1;
		}
	}

	if(filename){
		int error = access(filename, F_OK|R_OK);
		if(error == -1){
			perror("file specified could not be opened!");
			return 1;
		}
	}else if(!filename){
		filename = "./makefile";
		int error_1 = access("./makefile", F_OK|R_OK);
		if(error_1 == -1){
			filename = "./Makefile";
			int error_2 = access("./Makefile", F_OK|R_OK);
			if(error_2 == -1){
				perror("no makefile or Makefile to open!");
				return 1;
			}
		}
	}
	// process targets
	int target_num = argc - optind;
	if(target_num >=0){
		targets = malloc((target_num+1) * sizeof(char*));
		for(int i=0; i < target_num; i++){
			targets[i] = malloc(sizeof(strlen(argv[i + optind])+1));
			strcpy(targets[i], argv[i + optind]);
		}
	}
	targets[target_num] = NULL;

	rules = queue_create(-1, copy_pointer, destroy_pointer);
	parser_parse_makefile(filename, targets, parsed_new_target);
  //free targets
	for(int i=0; i<target_num+1; i++){
		if(targets[i] != NULL){
			free(targets[i]);
			targets[i] = NULL;
		}
	}
	free(targets);
	targets = NULL;
	//printf("thread num:%d\n", thread_num);
	Vector* sorted_rules = sort_rules();

	rules_ready = queue_create(-1, copy_pointer, destroy_pointer);
	for(size_t i=0; i<Vector_size(sorted_rules); i++){
		rule_t* temp = (rule_t*)Vector_get(sorted_rules, i);
		queue_push(rules_ready, temp);
	}
	rules_ready_size = Vector_size(sorted_rules);
	if(thread_num > rules_ready_size){
		thread_num = rules_ready_size;	
	}
	pthread_t threads[thread_num];
	//printf("rules_ready_size :%d\n", rules_ready_size);

	for(int i=0; i< thread_num; i++){
		pthread_create(&threads[i], NULL, execute, NULL);
	}
	//execute();

	for(int i=0; i< thread_num;i++){
		void* retval = NULL;
		pthread_join(threads[i], &retval);
	}
	Vector_destroy(sorted_rules);
	queue_destroy(rules);
	queue_destroy(rules_ready);
	pthread_mutex_destroy(&lock);
	return 0;
}


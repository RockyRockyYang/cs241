/**
 * MapReduce
 * CS 241 - Fall 2016
 */

#include "common.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void usage() { print_mr2_usage(); }

int main(int argc, char **argv) {
	if(argc != 7){
		perror("parameter passing error");
		exit(EXIT_FAILURE);	
	}
	int num_mapper = atoi(argv[5]);
	int num_reducer = atoi(argv[6]);
	int status_1[num_mapper], status_2[num_mapper], status_3, status_4[num_reducer];
	char* name_array[num_reducer + 2];

	int fds_mapper[2*num_mapper];
  for(int i=0; i< num_mapper; i++){
      if(pipe2(fds_mapper + 2*i, O_CLOEXEC)== -1){
        perror("pipe mapper created fail\n");
        exit(EXIT_FAILURE);
      }
      descriptors_add(fds_mapper[2*i]);
      descriptors_add(fds_mapper[2*i+1]);
  }
  // set up pipe for shuffler
  int fds_shuffler[2];
  if(pipe2(fds_shuffler, O_CLOEXEC)== -1){
      perror("pipe shuffler created fail");
      exit(EXIT_FAILURE);
  }
  descriptors_add(fds_shuffler[0]);
  descriptors_add(fds_shuffler[1]);

  int output = open(argv[2], O_RDWR|O_CREAT|O_TRUNC|O_CLOEXEC|O_APPEND, S_IRWXU);

  pid_t p_1[num_mapper], p_2[num_mapper];
	for(int i=0; i< num_mapper; i++){
		p_1[i] = fork();
		if(p_1[i] == -1){
      	perror("p_1 fail");
      	exit(EXIT_FAILURE);
    	}else if(p_1[i] == 0){
			dup2(fds_mapper[2*i+1], 1);
			descriptors_closeall(); 
			char count[10];
			sprintf(count, "%d",num_mapper);
			char idx[10];
			sprintf(idx, "%d", i);
      	if(execlp("./splitter", "./splitter",argv[1], count, idx, NULL) == -1){
          perror("splitter fail");
          exit(EXIT_FAILURE);
      	}
    	}else if(p_1[i] > 0){
			//nothing
    	}
	}
	for(int i=0; i< num_mapper; i++){
		p_2[i] = fork();
		if (p_2[i] == -1){
			perror("p_2 fail");
			exit(EXIT_FAILURE);
		}else if(p_2[i] == 0){
			dup2(fds_mapper[2*i], 0);
			dup2(fds_shuffler[1], 1);
			descriptors_closeall();
			if(execlp(argv[3], argv[3], (char*)NULL) == -1){
				perror("map fail");
				exit(EXIT_FAILURE);
			}
		}else if(p_2[i] > 0){
			//nothing
		}
	}

	for(int i=0; i< num_reducer + 1; i++){
			name_array[i] = malloc(20);
	}
	name_array[num_reducer+1] = NULL;
	strcpy(name_array[0], "./shuffler");
	for(int i=0; i< num_reducer; i++){
		sprintf(name_array[i+1], "./fifo_%d", i);
		mkfifo(name_array[i+1], S_IRWXU);
	}	

	pid_t p_3 = fork();
	if(p_3 == -1){
		perror("p_3 fail");
		exit(EXIT_FAILURE);
	}else if(p_3 == 0){
		dup2(fds_shuffler[0], 0);
		descriptors_closeall();
		if(execvp("./shuffler", name_array) == -1){
			perror("shuffler fail");
			exit(EXIT_FAILURE);
		}
	}else if(p_3 > 0){
		//nothing
		descriptors_closeall();
	}
	close(fds_shuffler[0]);
	
	pid_t p_4[num_reducer];
	int file_num[num_reducer];
	for(int i=0; i< num_reducer; i++){
		file_num[i] = open(name_array[i+1], O_RDONLY);
	}
	for(int i=0; i< num_reducer; i++){
		p_4[i] = fork();
		if(p_4[i] == -1){
			perror("p_4 fail");
			exit(EXIT_FAILURE);
		}else if(p_4[i] == 0){
			dup2(file_num[i], 0);
			dup2(output, 1);
			descriptors_closeall();
			if(execlp(argv[4], argv[4], (char*)NULL) == -1){
    	    	perror("reducer failure");
   	     		exit(EXIT_FAILURE);
    		}
		}
	}
	descriptors_closeall();
	close(output);
	for(int i=0; i< num_reducer; i++){
		close(file_num[i]);
		//unlink(name_array[i+1]);
		remove(name_array[i+1]);
	}
	waitpid(p_3, &status_3, 0);
	for(int i=0; i< num_mapper; i++){
		waitpid(p_1[i], &status_1[i], 0);
		waitpid(p_2[i], &status_2[i], 0);
	}
	for(int i=0; i< num_reducer; i++){
		waitpid(p_4[i], &status_4[i], 0);
	}

	for(int i=0; i< num_mapper; i++){
		if(WEXITSTATUS(status_1[i])){
			print_nonzero_exit_status(argv[3], WEXITSTATUS(status_1[i]));
		}
		if(WEXITSTATUS(status_2[i])){
			print_nonzero_exit_status(argv[3], WEXITSTATUS(status_2[i]));
		}
	}
	if(WEXITSTATUS(status_3)){
			print_nonzero_exit_status("shuffler", WEXITSTATUS(status_3));
	}
	for(int i=0; i< num_reducer; i++){
		if(WEXITSTATUS(status_4[i])){
			print_nonzero_exit_status(argv[4], WEXITSTATUS(status_4[i]));
		}
	}

	for(int i=0; i< num_reducer + 1; i++){
		free(name_array[i]);
	}
	print_num_lines(argv[2]);
	descriptors_destroy();
	return 0;
  // setup pipes
  // start mappers
  // start shuffler
  // start reducers
  // wait for everything to finish
}

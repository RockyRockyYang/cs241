/**
 * Lab: Utilities Unleashed
 * CS 241 - Fall 2016
 */

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "format.h"

extern char **environ;
#define BILLION 1000000000.0L

int main(int argc, char *argv[]) { 
	if(argc <= 1){
	print_time_usage();
	exit(EXIT_FAILURE);	
	}
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start); // start time	
	pid_t childid = fork();
	if(childid == -1){
		print_fork_failed();
		exit(EXIT_FAILURE);	
	}

	if(childid >0){
		int status;
		waitpid(childid, &status, 0);
		clock_gettime(CLOCK_MONOTONIC, &end); // end time
		double diff = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec)/BILLION;
		display_results(argv, diff);
	}
	if(childid ==0){		
		//execvp(argv[1], argv+1);
		if(execvp(argv[1], argv+1) <0){
			print_exec_failed();
			exit(EXIT_FAILURE);
		}
	}
	return 0; 
}

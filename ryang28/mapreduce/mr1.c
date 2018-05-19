#include "common.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void usage() { print_mr1_usage(); }

int main(int argc, char **argv){
	int num_mapper = atoi(argv[5]);
    int fds_mapper[2*num_mapper];
    for(int i=0; i< num_mapper; i++){
        if(pipe2(fds_mapper + 2*i, O_CLOEXEC)== -1){
            perror("pipe mapper created fail\n");
            exit(EXIT_FAILURE);
        }
        descriptors_add(fds_mapper[2*i]);
        descriptors_add(fds_mapper[2*i+1]);
    }
    int fds_reducer[2];
    if(pipe2(fds_reducer, O_CLOEXEC)== -1){
        perror("pipe reducer created fail");
        exit(EXIT_FAILURE);
    }
    descriptors_add(fds_reducer[0]);
    descriptors_add(fds_reducer[1]);
    int output = open(argv[2], O_RDWR| O_CREAT|O_TRUNC|O_CLOEXEC, S_IRWXU);
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
			dup2(fds_reducer[1], 1);
			descriptors_closeall();
			if(execlp(argv[3], argv[3], (char*)NULL) == -1){
				perror("map fail");
				exit(EXIT_FAILURE);
			}
		}else if(p_2[i] > 0){
			//nothing???
		}
	}
	pid_t p_3 = fork();
	if (p_3 == -1){
        perror("p_3 fail");
        exit(EXIT_FAILURE);
    }else if(p_3 == 0){
    	dup2(fds_reducer[0], 0);
    	dup2(output, 1);
    	descriptors_closeall();
    	if(execlp(argv[4], argv[4], (char*)NULL) == -1){
    	    perror("reducer failure");
   	     	exit(EXIT_FAILURE);
    	}
    }else{
        descriptors_closeall();
        close(output);
        int status_3;
		waitpid(p_3, &status_3, 0);
	}
	int status_1[num_mapper];
    int status_2[num_mapper];
	for(int i=0; i< num_mapper; i++){
		waitpid(p_1[i], &status_1[i], 0);
        waitpid(p_2[i], &status_2[i], 0);
	}
    print_num_lines(argv[2]);
	descriptors_destroy();
	return 0;
}

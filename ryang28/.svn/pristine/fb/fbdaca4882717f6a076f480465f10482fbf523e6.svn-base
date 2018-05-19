/**
 * Map Reduce 0 Lab
 * CS 241 - Fall 2016
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "utils.h"

int main(int argc, char **argv) {
  // Open the input file.
	int input = open(argv[1], O_RDONLY);
	// Open the output file.
    int output = open(argv[2], O_RDWR| O_CREAT|O_TRUNC, S_IRUSR);

	FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(argv[1], "r");
    if(fp == NULL)
       exit(EXIT_FAILURE);
    if ((read = getline(&line, &len, fp)) == -1) {
	 	fclose(fp);
		return 0;
	}
		fclose(fp);
	int fd[2];
    if(pipe2(fd, O_CLOEXEC)== -1){
		perror("pipe");
		exit(EXIT_FAILURE);
    }
  // Start the mapper.
    pid_t child_1 = fork();
    if (child_1 == -1){
		perror("child_1 fail");
		exit(EXIT_FAILURE);
    }else if(child_1 == 0){
		dup2(input, 0);
		dup2(fd[1], 1);
    	close(fd[1]);
    	if(execlp(argv[3], argv[3], (char*)NULL) == -1){
    		perror("map fail");
			exit(EXIT_FAILURE);
    	}
    }else if(child_1 >0){
    	pid_t child_2 = fork();
    	if (child_2 == -1){
		perror("child_2 fail");
		exit(EXIT_FAILURE);
		}else if(child_2 == 0){
			dup2(fd[0], 0);
			dup2(output, 1);
			close(fd[0]);
    		if(execlp(argv[4], argv[4], (char*)NULL) == -1){
    			perror("reducer failure");
				exit(EXIT_FAILURE);
    		}
		}else if(child_2 >0){
			close(fd[0]);
			close(fd[1]);
			close(input);
			close(output);
			int status_1;
			int status_2;
			waitpid(child_1, &status_1, 0);
			waitpid(child_2, &status_2, 0);
			if(!WIFEXITED(status_1)){printf("my_mapper exited with status 1\n");}
			if(!WIFEXITED(status_2)){printf("my_reducer exited with status 2\n");}
		}
    }
	print_num_lines(argv[2]);
	free(line);
	line = NULL;
  return 0;
}


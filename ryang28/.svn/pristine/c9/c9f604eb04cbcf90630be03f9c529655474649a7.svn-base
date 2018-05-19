/**
 * Lab: Utilities Unleashed
 * CS 241 - Fall 2016
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include "format.h"

extern char **environ;

int verify(char c){
  if(!isdigit(c) && !isalpha(c) && (c!='_'))
    return 0;
  else      
    return 1;
}
void assignment(char* token){
	int i=0;
	int j=0;
	for(;j<(int)strlen(token);j++){
		if(token[j] == '='){
			break;
		}
	}
	char* dest_var = strndup(token, j);
	for(i=0; i<(int)strlen(dest_var);i++){
		if(verify(dest_var[i]) ==0){
			free(dest_var);
			free(token);
			exit(EXIT_FAILURE);
		}
	}
  	char * result = calloc(1, 1);
	for(i = j + 1; i<(int)strlen(token) ;i++){
		char * start = &token[i + 1];
		if(token[i] == '%'){
			while(verify(token[i+1])) { i++; }
			char * end = &token[i];
	 	 	char * env_name = strndup(start, end - start + 1);
	 	 	char * env_val =  getenv(env_name);
	 	 	if(env_val!=NULL){
		 	 	result = realloc(result, strlen(result) + strlen(env_val) + 1);
		 	 	sprintf(result, "%s%s", result, env_val);
	 	 	}
		} else {
	 	 	result = realloc(result, strlen(result) + 2);	
		 	sprintf(result, "%s%c", result, token[i]);			
		}
	}
	setenv(dest_var, result, 1);
	free(dest_var);
}
char **strsplit(const char *str, const char *delim) {
  	char *s = strdup(str);
  	size_t tokens_alloc = 1;
  	size_t tokens_used = 0;
  	char **tokens = calloc(tokens_alloc, sizeof(char*));
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
  	free(s);
 	return tokens;
}

int main(int argc, char *argv[]) {
	pid_t childid = fork();
	if(childid  == -1){
		print_fork_failed();
		exit(EXIT_FAILURE);
	}
	if(childid >0){
		int status;
		waitpid(childid, &status, 0);
	}
	if(childid ==0){
		if(argc == 1){
			char** ptr = environ;
			while(*ptr){
				printf("%s\n", *ptr);
				ptr +=1;
			}
			exit(0);
		}
		else if(argc == 2){
			print_env_usage();
			exit(EXIT_FAILURE);
		}
		else if(argc >2){
			char* copy = strdup(argv[1]);
			char** output = strsplit(copy, ",");
			while(*output){
    			assignment(*output);
    			output++;
  			}
			free(copy);
			if(execvp(argv[2], argv+2) <0){
				print_exec_failed();
				exit(EXIT_FAILURE);
			}
		}			
	}
	return 0; 
}

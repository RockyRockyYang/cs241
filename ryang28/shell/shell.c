/**chine Problem: Shell
 * CS 241 - Fall 2016
 */
#include "format.h"
#include "log.h"
#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>

#define BUFFERSIZE 256

int is_build_in_command(const char* command);
void exec_build_in_command(const char* command);
void exec_external_command(const char* command);
void cleanup(int signal);
// global variable
Log* history;
char* path;

int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
	print_shell_owner("ryang28");
	history = Log_create();
	int history_flag = 0;
	int file_flag = 0;
	if(argv[2]){
		int opt;
		Log *f_log;
		while ((opt = getopt(argc, argv, "h:f:")) != -1) {
        	switch(opt){
        		case 'h':
            			history_flag = 1;
				Log_destroy(history);
            			history = Log_create_from_file(argv[2]);
            			break;
        		case 'f':
				file_flag = 1;
				f_log = Log_create_from_file(argv[2]);	
				for(size_t i=0; i< Log_size(f_log); i++){
					char* buffer = get_current_dir_name();
					print_prompt(buffer, getpid());
					free(buffer);
					const char* f_command = Log_get_command(f_log, i);
					if(is_build_in_command(f_command)){
						exec_build_in_command(f_command);
					}else{
						print_command(f_command);
						exec_external_command(f_command);
					}
				}
				Log_destroy(f_log);          
            			break;
        		case '?':
        			if(optopt == 'h'){
        				print_history_file_error();
        				return(1);
        			}
        			else if(optopt == 'f'){
        				print_script_file_error();
        				return(1);
        			}
		}
        }
    }
	if(file_flag == 1){
		Log_destroy(history);
		history = NULL;
 		return 0;
	}
	char* buffer_2 = get_current_dir_name();
	print_prompt(buffer_2, getpid());
	free(buffer_2);
	signal(SIGINT, SIG_IGN);
	char* line = NULL;
	size_t n = 0;
	ssize_t num;
	while((num = getline(&line, &n, stdin)) != -1){
		if(line[num -1] == '\n'){
			line[num-1] = '\0';
			num --;
		}
		if(is_build_in_command(line)){
			exec_build_in_command(line);
		}else{
			exec_external_command(line);
		}
		char* buffer_3 = get_current_dir_name();
		print_prompt(buffer_3, getpid());
		free(buffer_3);
	}
	
	if(history_flag){
		Log_save(history, argv[2]);
	}
	free(line);
	Log_destroy(history);
	history = NULL;
 	return 0;
}

int is_build_in_command(const char* command){
	if(strncmp(command, "cd", 2)== 0){
		return 1;
	}
	else if(strcmp(command, "!history")== 0){
		return 1;
	}	
	else if(command[0] == '#'){
		return 1;
	}
	else if(command[0] == '!'){
		return 1;
	}
	return 0;
}

void exec_build_in_command(const char* command){
	if(strncmp(command, "cd", 2)== 0){
		int exist = chdir(command + 3);
		if(exist == -1){
			print_no_directory(command + 3);
		}
		Log_add_command(history, command);
		return;	
	}
	else if(strcmp(command, "!history")== 0){
		size_t size = Log_size(history);
		if(size <1){
			print_history_file_error();
			return;
		}
		else{
			for(size_t index=0; index< size; index++){
				print_history_line(index, Log_get_command(history, index));
			}
			return;
		}
	}	
	else if(command[0] == '#'){
		size_t size = Log_size(history);
		if(size <1){
			print_history_file_error();
			return;
		}
		size_t idx = (size_t)atoi(command + 1);
		if(idx >= size){
			print_invalid_index();
			return;
		}
		const char* cmd = Log_get_command(history, idx);	
		if(is_build_in_command(cmd)){
			print_command(cmd);
			exec_build_in_command(cmd);	
		}else{
			print_command(cmd);
			exec_external_command(cmd);
		}
		return;
	}
	else if(command[0] == '!'){
		size_t size = Log_size(history);
		if(size <1){
			print_history_file_error();
			return;
		}
		char prefix[strlen(command)+1];
		strcpy(prefix, command + 1);
		int latest = -10;
		if(!isalpha(command[1])){
			latest = (int)(size-1);
		}
		for(int i=0; i< (int)size; i++){
			const char* str = Log_get_command(history, i);
			if(strncmp(prefix, str, strlen(prefix)) == 0){
				if(i > latest)
					latest = i;
			}
		}
		if(latest < 0){
			print_no_history_match();
			return;
		}
		const char* selected = Log_get_command(history, latest);
		if(is_build_in_command(selected)){
			print_command(selected);
			exec_build_in_command(selected);	
		}else{
			print_command(selected);
			exec_external_command(selected);
		}
	}
	else{
		print_exec_failed(command);
	}
}

void exec_external_command(const char* command){
	Log_add_command(history, command);
	int exist_signal = 0;
	char command_copy[BUFFERSIZE];
	strcpy(command_copy, command);
	if(command_copy[strlen(command_copy)-1] == '&'){
		exist_signal = 1;
		signal(SIGCHLD, cleanup);
		command_copy[strlen(command_copy)-1] = '\0';
	}
	size_t numtokens = 0;
	char** command_array = strsplit(command_copy, " ", &numtokens);
	pid_t childid = fork();
	if(childid <0){
		print_fork_failed();
		exit(EXIT_FAILURE);	
	}
	else if(childid >0){
		if(!exist_signal){
			int status;
			waitpid(childid, &status, 0);
		}else{}	// cases with &...
	}
	else if(childid == 0){
		pid_t curr_id = getpid();
		print_command_executed(curr_id);
		if(execvp(command_array[0], command_array) <0){
			print_exec_failed(command_array[0]);
			exit(EXIT_FAILURE);
		}
	}
	free_args(command_array);
}

void cleanup(int signal) {
  int status;
  while (waitpid((pid_t) (-1), &status, WNOHANG) > 0) {}
}


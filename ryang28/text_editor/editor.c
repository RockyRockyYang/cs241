/**
* Machine Problem: Text Editor
* CS 241 - Fall 2016
*/

#include "document.h"
#include "editor.h"
#include "format.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(int argc, char *argv[]) {
// TODO implement get_filename
// take a look at editor_main.c to see what this is used for
	if(argc == 1){
		return NULL;
	}
	else{
		return argv[1];
	}
}

void handle_display_command(Document *document, const char *command) {
// TODO implement handle_display_command
	size_t size = Document_size(document);
	if(size == 0){
		print_document_empty_error();
		return;
	}
	size_t numtokens = 0;
	char** command_array = strsplit(command, " ", &numtokens);
	if(command_array[1] == NULL){
		for(size_t i=1; i<(size+1);i++){
			print_line(document, i);
		}
	return;
	}
	else{
		size_t line_number = atoi(command_array[1]);
		if(line_number < 1 || line_number > size){
			invalid_line();
			return;
		}
		int start =(int)(line_number - 5);
		int end = (int)(line_number +5);
		if(start< 1)
			start = 1;
		if(end> (int)size)
			end = size;
		for(; start<= end; start++){
			print_line(document, start);
		}
	}
}

void handle_write_command(Document *document, const char *command) {
// TODO implement handle_write_command
	size_t size = Document_size(document);
	size_t numtokens = 0;
	char** lines = strsplit(command, "$", &numtokens);
	size_t numtokens_2 = 0;
	char** command_array = strsplit(lines[0], " ", &numtokens_2);
	int line_number = atoi(command_array[1]);
	if(line_number < 1){
		invalid_line();
		return;
	}
   	char* str_1 = strstr(lines[0], command_array[2]);
	if(line_number > (int)size){
		Document_insert_line(document, line_number, str_1);
	}
	else{
		Document_set_line(document, line_number, str_1);
	}
	int i=1;
	while(lines[i]){
		Document_insert_line(document, line_number+i, lines[i]);
		i++;
	}
}

void handle_append_command(Document *document, const char *command) {
// TODO implement handle_append_command
	size_t size = Document_size(document);
	size_t numtokens = 0;
	char** lines = strsplit(command, "$", &numtokens);
	size_t numtokens_2 = 0;
	char** command_array = strsplit(lines[0], " ", &numtokens_2);
	int line_number = atoi(command_array[1]);
	if(line_number < 1){
		invalid_line();
		return;
	}
   	char* str_1 = strstr(lines[0], command_array[2]);
	if(line_number > (int)size){
		Document_insert_line(document, line_number, str_1);
		int j=1;
		while(lines[j]){
			Document_insert_line(document, line_number+j, lines[j]);
			j++;
		}
		return;		
	}
	const char* original = Document_get_line(document, line_number);
	char* copy = malloc(strlen(original)+1);
	strcpy(copy, original);
	strcat(copy, str_1);
	Document_set_line(document, line_number, copy);
	int i=1;
	while(lines[i]){
		Document_insert_line(document, line_number+i, lines[i]);
		i++;
	}
}

void handle_delete_command(Document *document, const char *command) {
// TODO implement handle_delete_command
	size_t size = Document_size(document);
	if(size == 0){
		print_document_empty_error();
		return;
	}
	size_t numtokens = 0;
	char** command_array = strsplit(command, " ", &numtokens);
	int line_number = atoi(command_array[1]);
	if(line_number < 1 || line_number > (int)size){
		invalid_line();
		return;
	}
	Document_delete_line(document, line_number);
}

void handle_search_command(Document *document, const char *command) {
// TODO implement handle_search_command
	size_t size = Document_size(document);
	if(size == 0){
		print_document_empty_error();
		return;
	}
	for(int i=1; i<= (int)size; i++){
		const char* line = Document_get_line(document, i);
		char* ret = strstr(line, command+1);
		if(ret){
			print_search_line(i, line, ret, command+1);
		}
	}
}

void handle_save_command(Document *document, const char *filename) {
// TODO implement handle_save_command
	Document_write_to_file(document, filename);
}


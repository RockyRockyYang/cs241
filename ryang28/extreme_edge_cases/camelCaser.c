/** * Chatroom Lab
 * CS 241 - Fall 2016
 */
#include "camelCaser.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//helper functions
void generate_sentence(char** empty_array, char* line){
	int start = 0;
	int end = 0;
	int index = 0;
	int length = strlen(line);
	while(end < length){
		if(ispunct((int)line[end])){
			line[end] = '\0';		// turn punctuation into null
			empty_array[index] = line + start;
			index++;
			if(isspace((int)line[end+1])){
				start = end+2;
				end  = start;
			}
			else{
				start = end+1;
				end = start;
			}
		}
		end++;
	}
}

void camel_funct(char* arr){
		int i = 0;
    while(arr[i]){
    	arr[i] = tolower(arr[i]);
    	i++;
			//printf("%p\n", *temp);
    }
    int j = 1;
    while(arr[j]){
    	if(isspace(arr[j])&&isalpha(arr[j+1])){
    		arr[j+1] = toupper(arr[j+1]);
    	}
    	j++;
	}
	//printf("%s\n", arr);
   	char* a = arr;
   	char* b = arr;
   	while(*b != 0){
     	*a = *b++;
     	if(*a != ' ')
       		a++;
   	}
   	*a = 0;
}

char **camel_caser(const char *input_str) {
  if(input_str == 0)
    return NULL;
  char* input_copy = malloc(strlen(input_str)+1);
 //memset(input_copy, '\0', sizeof(input_copy));
  strcpy(input_copy, input_str);
  int n = 0;		// count how many sentence
  for(int i= 0; i< (int)strlen(input_str); i++){
  	if(ispunct(input_str[i]))
  		n++;
  }
  char** input_array = malloc((n+1)*sizeof(char*));
  for(int i=0; i<(n+1); i++){
  		input_array[i] = NULL;
  }
	if(n== 0)
		return input_array;
  // split strings into sentences(input_arrays)
  generate_sentence(input_array, input_copy);
  // create camel
  for(int i=0; i< n; i++){
			if(input_array[i] != NULL)
  			camel_funct(input_array[i]);
  }
  return input_array;
}

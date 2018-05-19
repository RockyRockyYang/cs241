/**
 * Chatroom Lab
 * CS 241 - Fall 2016
 */
#include "camelCaser_tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"


int compare_output(char **output, char ** correct){
    int ret = 1;
    if(output != NULL){
        char ** line = output;
  		char ** c = correct;	
    	while(*line && *c){
	    	if(strcmp(*line, *c)){
				ret = 0;
				break;
	    	}
        	line++;
	    	c++;
    	}
	if(*c || *line) ret = 0;
    } else{
        printf("NULL POINTER ERROR\n");
    }
    return ret;
}


/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser(char **(*camelCaser)(const char *)) {
  // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
    char * inputs[] = {
        " ***HeLLo 8SB D7 ??  241WoRLd  3traceback windowmakingmoney6789... 2w   ",
        NULL
    };
    int count = 0;
    char ** input = inputs;
    char **output = camelCaser(*input);
    //char **mark = output;
    char * correct_output[] = {
	"", "", "", "hello8SbD7", "", "241world3TracebackWindowmakingmoney6789", "", "", NULL
    };
    count = count + compare_output(output, correct_output);
    /*while(*output){
	free(*output);
	output++;	
    }
    free(mark);
	*/
    char **out_2 = camelCaser(NULL);
    if(out_2 == NULL) count += 1; 
    if(count == 2){
		return 1;
	}else{
		return 0;
	}
}

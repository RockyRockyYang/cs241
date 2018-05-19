#include <stdio.h>
#include <stdlib.h>
#include "camelCaser.h"

void print_output(char **output){
    printf("Got the following:\n");
    if(output){
        char ** line = output;
        while(*line){
            printf("\t'%s'\n", *line);
            line++;
        }
    } else{
        printf("NULL POINTER!\n");
    }
    printf("-----------------------------------------------------------------------------------------\n");
}

void print_input(char * input){
    printf("testing: \n\t'%s'\n\n", input);
}

int main() {
    char * inputs[] = {
        "Hello World.",
        "Code smell can be ignored with INCREDIBLE use of air freshener. God objects are the new religion.",
       ".?!#@","abcdefg","space.  space","WHERE THINGS GOES WRONG? I DON'T UNDERSTAND","Career fair or lab... What??","Alphabet is good test! abcdefg","Hello.World","abc\ncdfe.",NULL
    };

    char ** input = inputs;
    while(*input){
        print_input(*input);
        char **output = camel_caser(*input);
        print_output(output);
        input++;
    }
    return 0;
}

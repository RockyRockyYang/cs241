/**
 *MapReduce
 * CS 241 - Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "utils.h"

void usage() {
  fprintf(stderr, "shuffler destination1 destination2 ...\n");
  fprintf(stderr, "where destination1..n are files on the filesystem to which "
                  "the shuffler will write its output\n");
}

int main(int argc, char *argv[]) {
  // read from stdin
  // hash the key for the input line
  // send them to the correct output file (output files are given as command
  // line arguments

  if (argc < 2) {
    usage();
    exit(1);
  }
/*	fprintf(stderr, "which process am i in?\n");
  int n = argc - 1;
	FILE* outf[n];
	for(int i=0; i< n; i++){
		outf[i] = fopen(argv[1 + i], "w");
		assert(outf[i] == NULL);
	}
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
	fprintf(stderr, "which process am i in?\n");	
  while((read = getline(&line, &len, stdin)) != -1){
    char *key = NULL;
    char *value = NULL;
    int ret = split_key_value(line, &key, &value);
    if(ret == -1){
      perror("split failure");
      exit(1);
    }
    int idx = hashKey(key) % n;
    fprintf(outf[idx], "%s: %s\n", key, value);
  }
	for(int i=0; i< n; i++){
		fclose(outf[i]);
	}
  return 0;
*/
char* lineptr = NULL;
	size_t len = 0;
	int get_ret = 0;
	int files = argc-1;
	
	FILE* output_files[files];
	for(int i=0; i<files; i++) {
		output_files[i] = fopen(argv[i+1], "a+");
	}

	while((get_ret = getline(&lineptr, &len, stdin))!=-1) {
		char* key = NULL;
		char* value = NULL;
		int if_true = split_key_value(lineptr, &key, &value);
		if(if_true == 1) {
			FILE* outf = output_files[hashKey(key)%files];
			fprintf(outf, "%s: %s\n", key, value);
		}
	}

	for(int i=0; i<files; i++) {
		fclose(output_files[i]);
	}
  return 0;

}

/**
 * Mad Mad Access Pattern
 * CS 241 - Fall 2016
 */
#include "tree.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
*/
int get_size(FILE* fp, int offset){
	fseek(fp, offset+16, SEEK_SET);
	int count = 0;
	while(1){
		int c = fgetc(fp);
		count++;
		if(c == '\0'){
			break;
		}
	}
	return count+16;
}

BinaryTreeNode* find_word(char* word, FILE* fp, int offset){
	if(offset == 0){
		return NULL;
	}
	int node_size = get_size(fp, offset);
	BinaryTreeNode *node = malloc(node_size);
	fseek(fp, offset, SEEK_SET);
	fread(node, node_size, 1, fp);
	int ret = strcmp(word, node->word);
	if(ret == 0){
		return node;
	}else if(ret < 0){
		return find_word(word, fp, node->left_child);
	}else{
		return find_word(word, fp, node->right_child);
	}
}

int main(int argc, char **argv) { 
	if(argc <= 2){
		printArgumentUsage();
		exit(1);
	}
	FILE* fp = fopen(argv[1], "r");
	if(!fp){
		openFail(argv[1]);
		exit(2);
	}
	fseek(fp, 0, SEEK_SET); // Move to the start of the file 
	char buffer[4];
	fread(buffer, 4, 1, fp);
	if(strcmp(buffer, "BTRE")!= 0){
		formatFail(argv[1]);
		exit(2);
	}
	int word_num = argc - 2;
	for(int i=0; i< word_num; i++){
		char* word = argv[2+i];
		BinaryTreeNode* result = find_word(word, fp, 4);
		if(result == NULL){
			printNotFound(word);
		}else{
			uint32_t count = result->count;
			float price = result->price;
			printFound(word, count, price);
		}
	}
	fclose(fp);
	return 0; 
}

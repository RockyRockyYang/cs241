/**
 * Mad Mad Access Pattern
 * CS 241 - Fall 2016
 */
#include "tree.h"
#include "utils.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses mmap to access the data.

  ./lookup2 <data_file> <word> [<word> ...]
*/

BinaryTreeNode* find_word(char* word, char* addr, int offset){
	if(offset == 0){
		return NULL;
	}
	BinaryTreeNode* node = (BinaryTreeNode*)(addr + offset); 
	int ret = strcmp(word, node->word);
	if(ret == 0){
		return node;
	}else if(ret < 0){
		return find_word(word, addr, node->left_child);
	}else{
		return find_word(word, addr, node->right_child);
	}
}

int main(int argc, char **argv) { 
	if(argc <= 2){
		printArgumentUsage();
		exit(1);
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		openFail(argv[1]);
		exit(2);
	}
	struct stat st;
	stat(argv[1], &st);
	int size =(int)st.st_size;
	char* addr = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
	if(strncmp(addr, "BTRE", 4)!= 0){
		formatFail(argv[1]);
		exit(2);
	}	
	int word_num = argc - 2;
	for(int i=0; i< word_num; i++){
		char* word = argv[2+i];
		BinaryTreeNode* result = find_word(word, addr, 4);
		if(result == NULL){
			printNotFound(word);
		}else{
			uint32_t count = result->count;
			float price = result->price;
			printFound(word, count, price);
		}
	}
	close(fd);
	return 0; 

}

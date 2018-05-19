/**
* Finding Filesystems
* CS 241 - Fall 2016
*/
#include "format.h"
#include "fs.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void fs_ls(file_system *fs, char *path) {
  inode* node = get_inode(fs, path);
  if(node == NULL){
    print_no_file_or_directory();
    return;
  }
  if(is_file(node)){
    char* filename = path;
    parent_directory(fs, path, &filename);
    print_file(filename);
  }
  if(is_directory(node)){
    int num_directory = node->size / 256;
    int remain = num_directory;
    char* ptr = (char*)(&fs->data_root[node->direct_nodes[0]]);
    while(remain >0){
      dirent to_fill;
      make_dirent_from_string(ptr, &to_fill);

      char* name = to_fill.name;
      inode_number idx = to_fill.inode_num;
      inode* detect = fs->inode_root + idx;
      if(detect == NULL){
        print_no_file_or_directory();
      }
      else if(is_file(detect)){
        print_file(name);
      }
      else if(is_directory(detect)){
        print_directory(name);
      }

      ptr += 256;
      remain--;
    }
  }
}

void fs_cat(file_system *fs, char *path) {
  inode* curr = get_inode(fs, path);
  if(curr == NULL){
    print_no_file_or_directory();
    return;
  }
  size_t last_size = sizeof(data_block);  
  size_t num_block = curr->size / sizeof(data_block);
  if((curr->size % sizeof(data_block))!= 0){
    num_block += 1;
    last_size = curr->size % sizeof(data_block);
  }
  int indirect = 0;
  if(num_block > NUM_DIRECT_INODES){
    indirect = 1;
  }

  if(!indirect){
    for(size_t i=0; i< num_block-1; i++){
      write(1, &fs->data_root[curr->direct_nodes[i]], sizeof(data_block));
    }
    write(1, &fs->data_root[curr->direct_nodes[num_block-1]], last_size); 
  }else{
    for(size_t i=0; i< NUM_DIRECT_INODES; i++){
      write(1, &fs->data_root[curr->direct_nodes[i]], sizeof(data_block));
    }
    size_t left_block = num_block - NUM_DIRECT_INODES;
		inode* left_node = fs->inode_root + curr->single_indirect;
    for(size_t i=0; i< left_block-1; i++){
      write(1, &fs->data_root[left_node->direct_nodes[i]], sizeof(data_block));
    }
    write(1, &fs->data_root[left_node->direct_nodes[left_block-1]], last_size);
  }
}

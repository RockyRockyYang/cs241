/**
 *chatroom Lab
 * CS 241 - Fall 2016
 */
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include "utils.h"

static const size_t MESSAGE_SIZE_DIGITS = 4;

char *create_message(char *name, char *message) {
  int name_len = strlen(name);
  int msg_len = strlen(message);
  char *msg = calloc(1, msg_len + name_len + 4);
  sprintf(msg, "%s: %s", name, message);

  return msg;
}

ssize_t get_message_size(int socket) {
  int32_t size;
  ssize_t read_bytes =
      read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
  if (read_bytes == 0 || read_bytes == -1)
    return read_bytes;

  return (ssize_t)ntohl(size);
}

ssize_t write_message_size(size_t size, int socket) {
  // Your code here
  size = htonl(size);
  ssize_t write_bytes = 
      write_all_to_socket(socket, (char*)&size, MESSAGE_SIZE_DIGITS);
  if(write_bytes == 0 || write_bytes == -1){
    return write_bytes;
  }
  return (ssize_t)size;
}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
  // Your Code Here
  ssize_t read_byte;
  ssize_t accumulate = 0;
  while(count > 0){
    read_byte = read(socket, buffer, count);
    if(read_byte >0){
      buffer += read_byte; //move the pointer
      count -= read_byte;
      accumulate += read_byte;
    }else if(read_byte == 0){
      return 0;
    }else if(read_byte == -1 && errno == EINTR){
      continue;
    }else if(read_byte == -1 && errno != EINTR){
      return -1;
    }
  }
  return accumulate;
}

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
  // Your Code Here
  ssize_t write_byte;
  ssize_t accumulate = 0;
  while(count > 0){
    write_byte = write(socket, buffer, count);
    if(write_byte >0){
      buffer += write_byte;
      count -= write_byte;
      accumulate += write_byte;
    }else if(write_byte == 0){
      return 0;
    }else if(write_byte == -1 && errno == EINTR){
      continue;
    }else if(write_byte == -1 && errno != EINTR){
      return -1;
    }
  }
  return accumulate;
}

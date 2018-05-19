/**
 * Machine Problem: Vector
 * CS 241 - Fall 2016
 */

#include "document.h"
#include <stdio.h>
// test your document here
int main() { 
  Document* doc  = Document_create();
  Document_insert_line(doc, 1, "a");
  Document_insert_line(doc, 3, "c");
  Document_insert_line(doc, 7, "g");
  Document_insert_line(doc, 11, "k");
  Document_insert_line(doc, 27, "gg wp");
  Document_write_to_file(doc, "test.txt");
  const char* ptr;
  for (size_t i = 0; i < Document_size(doc); i++) {
    ptr = Document_get_line(doc, i+1);
    if(ptr)
      printf("%s\n", ptr);
    else
      printf("\n");
  }
  Document_destroy(doc);
  doc = Document_create_from_file("test.txt");
  printf("Size of read file %zu\n", Document_size(doc));
  Document_write_to_file(doc, "copy.txt");
  Document_destroy(doc);
  doc = NULL;
  return 0;
}

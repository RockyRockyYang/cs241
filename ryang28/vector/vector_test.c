#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void print_vector(Vector * vector) {
  size_t size = Vector_size(vector);
  size_t capacity = Vector_capacity(vector);
  printf("vector size: %zu, vector capacity: %zu\n", size, capacity);
  printf("|");
  for (size_t i = 0; i < size; i++) {
    printf("%s|", Vector_get(vector, i));
  }
  for (size_t i = size; i < capacity; i++) {
    printf("__|");
  }
  printf("\n-----------------------------------------\n\n\n");
}

void *my_copy_ctor(void *elem) {
  char *str = elem;
  assert(str);
  return strdup(str);
}

void my_destructor(void *elem) { free(elem); }

int main() {
  printf("Creating a new vector\n");
  Vector *vector = Vector_create(my_copy_ctor, my_destructor);
  //Vector *vector = Vector_create();
  print_vector(vector);
  printf("Adding 10 elements\n");
  for (size_t i = 0; i < 10; i++) {
    // this gets copied in add, so its fine to declare in loop
    char str[2];
    str[0] = 'A' + i;
    str[1] = '\0';
    printf("\tAdding '%c' at index %zu\n", str[0], i);
    Vector_append(vector,str);
    if(i == 0 || i == 9){
      print_vector(vector);
    }
  }
  printf("Now I am adding an 11th element .. watch the vector capacity increase\n");
  char *c  = "K";
  printf("\tAdding 'K' at index 10\n");
  Vector_append(vector, c);
  print_vector(vector);
  printf("Now I am inserting NULL into index 5\n");
  Vector_insert(vector, 5, NULL);
  print_vector(vector);
  char *something = "something";
  printf("Now I am inserting another thing into index 5\n\t Notice how NULL shifted down instead of overwritten\n");
  Vector_insert(vector, 5, something);
  print_vector(vector);
  printf("Now I am inserting way past the end of the array (at index 42)\n\tNotice how the vector resizes to accommodate the last element\n");
  char *way_past = "way past the end";
  Vector_insert(vector, 42, way_past);
  print_vector(vector);
  printf("Now I will demonstrate the 3 resize scenarios:\n\n\n");
  printf("Resizing to less than size and capacity (15 in this example)\n\tNotice that things got deallocated\n");
  Vector_resize(vector, 15);
  print_vector(vector);
  printf("Resizing to greater than size but less than capacity (21 in this example)\n\tNotice that indices 15 to 21 but padded with NULLS (they are valid elements\n");
  Vector_resize(vector, 21);
  print_vector(vector);
  printf("Resizing to greater than size AND capacity (64 in this example)\n\tNotice that the vector's capacity had to double to accomodate the valid NULLs\n");
  Vector_resize(vector, 64);
  print_vector(vector);
  Vector_destroy(vector);
  return 0;
}

/**
 * Machine Problem: Vector
 * CS 241 - Fall 2016
 */

/* An automatically-expanding array of strings. */
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_CAPACITY 10

/*
 * Vector structure
 * Do not modify the structure
 * array: Void pointer to the beginning of an array of void pointers to
 * arbitrary data.
 * size: The number of elements in the vector. This is the number of actual
 * objects held in the vector, which is not necessarily equal to its capacity.
 * capacity: The size of the storage space currently allocated for the vector,
 * expressed in terms of elements.
 * copy_constructor: the function callback for you to define the way you want
 * to copy elements
 * destructor:  the function callback for you to define the way you want to
 * destroy elements
 */

struct Vector {
  copy_constructor_type copy_constructor;
  destructor_type destructor;

  void **array;
  size_t size;
  size_t capacity;
};


Vector *Vector_create(copy_constructor_type copy_constructor,
                      destructor_type destructor) {
  // your code here
  Vector *vector = malloc(sizeof(Vector));
	vector->copy_constructor = copy_constructor;
  vector->destructor = destructor;
  vector->array = calloc(INITIAL_CAPACITY, sizeof(void *));
  vector->size = 0;
  vector->capacity = INITIAL_CAPACITY; 
  return vector;
}

void Vector_destroy(Vector *vector) {
  assert(vector != NULL);
  // your code here
  for(size_t i = 0; i < vector->size; i++){
    vector->destructor(vector -> array[i]);
  }
  free(vector->array);
  free(vector);
  vector = NULL;
}

size_t Vector_size(Vector *vector) {
  assert(vector != NULL);
  // your code here
  return vector->size;
}

size_t Vector_capacity(Vector *vector) {
  assert(vector != NULL);
  // your code here
  return vector->capacity;
}

void Vector_resize(Vector *vector, size_t new_size) {
	assert(vector);
	assert(new_size >= 0);
  // your code here
  size_t old_size = vector->size;
  vector->size = new_size; // change the size 
  if(new_size > vector->capacity){
    size_t large_capacity = 2 * vector->capacity; 
    while(large_capacity < new_size) {
			large_capacity *= 2;
		}
    vector->capacity = large_capacity; 
    vector->array = realloc(vector->array, (vector->capacity) * sizeof(void *));
		size_t i = old_size;    
		for(; i < vector->capacity; i++){
      vector->array[i] = NULL; 
    }
		return; 
  }else if(new_size <= (vector->capacity)/4){
    if(vector->capacity > 10){
      size_t small_capacity = (vector->capacity) / 2;
      while(small_capacity > 2 * new_size) {
				small_capacity /= 2;
			}
      if(small_capacity < 10) {
				small_capacity = 10;
			}
	  size_t i = vector->capacity - 1;
      for(; i >= small_capacity; i--){
        if(vector->array[i] != NULL){
          vector->destructor(vector->array[i]);
          vector->array[i] = NULL;
        }
      }
      vector->capacity = small_capacity;
      vector->array = realloc(vector->array, (vector->capacity) * sizeof(void *));
    }
	size_t i = vector->capacity - 1;   
    for(; i >= new_size; i--){
      if(vector->array[i] != NULL){
        vector->destructor(vector->array[i]);
        vector->array[i] = NULL;
      }
    }
  }else{
    if(new_size < old_size){
	size_t i = old_size - 1;
      for(; i >= new_size; i--){
        if(vector->array[i] != NULL){
	  vector->destructor(vector->array[i]);
	  vector->array[i] = NULL;
        }
      }
    }
  }
}

void Vector_set(Vector *vector, size_t index, void *elem) {
  assert(vector);
  //printf("Vector current size is %zu",Vector_size(vector));
  // your code here
  assert(index < vector->size);
  assert(index >=0);

  if(vector->array[index] != NULL){
      vector->destructor(vector->array[index]);
  }
  if(elem == NULL) {
		vector->array[index] = NULL;
	}else {
		vector->array[index] = vector->copy_constructor(elem);
	} 
}

void *Vector_get(Vector *vector, size_t index) {
  assert(vector);
	assert(index < vector->size);
  assert(index >=0);
  assert(vector->array);
  // your code here
	if(!(vector->array)[index]){
    return NULL;
  }
  return (vector->array)[index];
}

void Vector_insert(Vector *vector, size_t index, void *elem) {
  assert(vector);
  // your code here
  assert(index >=0);
  if(index > (vector->size-1)){
    Vector_resize(vector, index + 1);
  } 
  else{
    Vector_resize(vector, vector->size + 1);
		long i = (long)vector->size - 1;
    for(; i > (long)index; i--){
      vector->array[i] = vector->array[i-1];
    }
  }
  if(elem == NULL) vector->array[index] = NULL;
  else vector->array[index] = vector->copy_constructor(elem); 
}

void Vector_delete(Vector *vector, size_t index) {
  assert(vector);
  // your code here, what asserts might you want?
  assert(index< vector->size);
  assert(index >=0);
  if(vector->array[index] != NULL)
    vector->destructor(vector->array[index]);
  size_t i = index;
  for(; i< vector->size-1; i++){
    void* temp = Vector_get(vector, i+1);
    Vector_set(vector, i, temp);
  }
  Vector_set(vector, vector->size-1, NULL);
  Vector_resize(vector, vector->size-1);
}

void Vector_append(Vector *vector, void *elem) { //append the element to array[size]
  assert(vector);
  // your code here
  Vector_resize(vector, vector->size + 1);
  Vector_set(vector, vector->size -1, elem);
}

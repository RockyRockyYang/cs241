/**
 * Machine Problem: Malloc
 * CS 241 - Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STD_SIZE 64
typedef struct _meta_data{
  size_t size;
  int available;
  struct _meta_data* next;
  struct _meta_data *prev;
} meta_data;

meta_data *head = NULL;
void* start;
int flag = 1;

meta_data* request_block(size_t size);
meta_data* splitblock(meta_data* chosen, size_t req_size);
void merge_behind(meta_data* curr, meta_data* neighbor);
void merge_front(meta_data* curr, meta_data* neighbor);
void merge_both(meta_data* curr, meta_data* front, meta_data* behind);
/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
	void* ptr = malloc(num * size);
	if(!ptr){return NULL;}
  	return memset(ptr, 0, num * size);
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
void *malloc(size_t size) {
	if(flag == 1){
		start = sbrk(0);
		flag = 0;
	}
	meta_data *p = head;
	meta_data *chosen = NULL;
	while(p != NULL){
		if(p->available && p->size >= size){
			if(chosen == NULL || (chosen && p->size < chosen->size)){
				chosen = p;
				break;
			}
		}
		p = p->next;
	}
	if(!chosen){chosen = request_block(size);}	
	if((chosen->size-size) > (sizeof(meta_data) + sizeof(size_t))){
		meta_data* new_block = splitblock(chosen, size);
		if(chosen->prev != NULL && chosen->next != NULL){
			new_block->next = chosen->next;
			new_block->prev = chosen->prev;
			chosen->prev->next = new_block;
			chosen->next->prev = new_block;
		}else if(chosen->prev != NULL && chosen->next == NULL){
			new_block->next = chosen->next;
			new_block->prev = chosen->prev;
			chosen->prev->next = new_block;
		}else if(chosen->prev == NULL && chosen->next != NULL){
			new_block->next = chosen->next;
			new_block->prev = chosen->prev;
			chosen->next->prev = new_block;
			head = new_block;
		}else{
			new_block->next = chosen->next;
			new_block->prev = chosen->prev;
			head = new_block;
		}
	}else{
		if(chosen->prev != NULL && chosen->next != NULL){
			chosen->prev->next = chosen->next;
			chosen->next->prev = chosen->prev;
		}else if(chosen->prev == NULL && chosen->next != NULL){
			chosen->next->prev = NULL;
			//head = NULL;
			head = chosen->next;
		}else if(chosen->prev != NULL && chosen->next == NULL){
			chosen->prev->next = NULL;
		}else{head = NULL;}
	}
	chosen->available = 0;
	return chosen+1;
}

meta_data* splitblock(meta_data* chosen, size_t req_size){
	meta_data* new_block = (void*)chosen + sizeof(meta_data) + req_size + sizeof(size_t);
	new_block->size = chosen->size - sizeof(meta_data) - req_size - sizeof(size_t);
	chosen->size = req_size;
	// adding size to the end of chosen
	void* end_size = (void*)new_block - sizeof(size_t);
	*(size_t*)end_size = req_size;
	// adding size to the end of new_block
	void* end_size_2 = (void*)new_block + sizeof(meta_data)+ new_block->size;
	*(size_t*)end_size_2 = new_block->size;

	new_block->available = 1;
	return new_block;
}

meta_data* request_block(size_t size){
	size_t alloc_size = STD_SIZE;
	//while(alloc_size < size){alloc_size *= 2;}
	alloc_size = size;
	void* temp = sbrk(sizeof(meta_data) + alloc_size + sizeof(size_t));
	if(temp == (void*)-1){return NULL;}
	meta_data* block = (meta_data*)temp;
	block->size = alloc_size;
	block->available = 1;
	// adding size to the end
	void* end_size = (void*)block + sizeof(meta_data) + alloc_size;
	*(size_t*)end_size = alloc_size;

	if(head == NULL){
		head = block;
		block->prev = NULL;
		block->next = NULL;
	}else{
		meta_data* temp = head;
		block->next = temp;
		block->prev = NULL;
		temp->prev = block;
		head = block;
	}
	return (meta_data*)block;
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
	if(ptr == NULL){return;}
	meta_data* block = ptr;
	block = block - 1;//head of block
	block->available = 1;
	void* front_size_ptr = (void*)block - sizeof(size_t);
	size_t front_size = *(size_t*)front_size_ptr;
	meta_data* neighbor_front = front_size_ptr - front_size - sizeof(meta_data);
	meta_data* neighbor_behind = ptr + block->size + sizeof(size_t);
	meta_data* tmp = sbrk(0);

	int flag_front = 0;
	int flag_behind = 0;
	if(block != start){flag_front = 1;}
	if(neighbor_behind != tmp){flag_behind = 1;}

	if(flag_front && flag_behind && neighbor_front->available && neighbor_behind->available){
		merge_both(block, neighbor_front, neighbor_behind);
	}/*else if(flag_front && neighbor_front->available){
		merge_front(block, neighbor_front);
	}*/
	else if(flag_behind && neighbor_behind->available){
		merge_behind(block, neighbor_behind);
	}
	else{ // no merge, set it head
		if(head == NULL){
			head = block;
			block->next = NULL;
			block->prev = NULL;
		}else{
			block->next = head;
			block->prev = NULL;
			head->prev = block;
			head = block;
		}
	}
}

// merge with next neighbor
void merge_behind(meta_data* curr, meta_data* neighbor){
	//fprintf(stderr, "merge!\n");
	curr->size += sizeof(meta_data) + neighbor->size + sizeof(size_t);
	// adding size to the end
	void* end_size = (void*)neighbor + sizeof(meta_data) + neighbor->size;
	*(size_t*)end_size = curr->size;

	if(neighbor->prev != NULL && neighbor->next != NULL){
		curr->prev = neighbor->prev;
		curr->next = neighbor->next;
		neighbor->prev->next = curr;
		neighbor->next->prev = curr;
	}else if(neighbor->prev != NULL && neighbor->next == NULL){
		curr->prev = neighbor->prev;
		curr->next = NULL;
		neighbor->prev->next = curr;
	}else if(neighbor->prev == NULL && neighbor->next != NULL){
		curr->prev = NULL;
		curr->next = neighbor->next;
		neighbor->next->prev = curr;
		head = curr;
	}else{
		curr->prev = NULL;
		curr->next = NULL;
		head = curr;
	}	
}
// merge with front neighbor
void merge_front(meta_data* curr, meta_data* neighbor){
	neighbor->size += sizeof(meta_data) + curr->size + sizeof(size_t);
	// adding size to the end
	void* end_size = (void*)curr + sizeof(meta_data) + curr->size;
	*(size_t*)end_size = neighbor->size;
}
//merge with both neighbor
void merge_both(meta_data* curr, meta_data* front, meta_data* behind){
	front->size += 2*sizeof(meta_data) + 2*sizeof(size_t) + curr->size + behind->size;
	// adding size to the end
	void* end_size = (void*)behind + sizeof(meta_data) + behind->size;
	*(size_t*)end_size = front->size;
	//fuse pointers next to behind
	if(behind->prev != NULL && behind->next != NULL){
		behind->prev->next = behind->next;
		behind->next->prev = behind->prev;
	}else if(behind->prev != NULL && behind->next == NULL){
		behind->prev->next = NULL;
	}else if(behind->prev == NULL && behind->next != NULL){
		behind->next->prev = NULL;
		head = behind->next;
	}else{head = NULL;}
	// do nothing since front is already linked
}
/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
	if(!ptr){ return malloc(size);}
	if(size == 0){
		free(ptr);
		return NULL;
	}	
	meta_data* block = ptr;
	block = block -1;
	if(block->size >= size){return ptr;}
	void* new_ptr = malloc(size);
	if(!new_ptr){return NULL;}
	memcpy(new_ptr, ptr, block->size);
	free(ptr);
	return new_ptr;
}


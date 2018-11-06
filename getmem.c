/*
 * getmem.c - Code to dynamically allocate space on the heap as a pointer
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

# include <stdio.h>
# include <stdlib.h>
# include <inttypes.h>
# include "mem_impl.h"
# include "mem.h"

# define THRESHOLD_BLOCK_SIZE 16
# define DEFAULT_REQUEST_BLOCK_SIZE 8000

// Initialze the freelist to be empty
FreeNode* head = NULL;
// Keep track of total bytes malloced
uintptr_t total_malloc_size = 0;
// Helper method that rounds up the given size to  the nearest
// memory alignment size
uintptr_t alignedSize(uintptr_t size);

// Frees space on the heap for a pointer and returns.
// Takes as an argument:
//     - an unsigned int "size" which is the size of the space in bytes
// Returns a pointer, or NULL if allocation failed or given size is
// not positive
void* getmem(uintptr_t size) {
  if (size <= 0) {
    return NULL;
  }
  check_heap();
  uintptr_t reqSize = alignedSize(size + sizeof(FreeNode));
  FreeNode* temp = head;
  FreeNode* prev = temp;
  while (temp != NULL && temp->size < size) {
    prev = temp;
    temp = temp->next;
  }
  // get more space to the head or where the address is appropriate if not found
  if (temp == NULL) {
    if (reqSize < DEFAULT_REQUEST_BLOCK_SIZE) {
      reqSize = DEFAULT_REQUEST_BLOCK_SIZE;
    }
    FreeNode* newBlock = (FreeNode*) malloc(reqSize);
    // report malloc error
    if (newBlock == NULL) {
      fprintf(stderr, "Malloc failed to allocate space.\n");
      return NULL;
    }
    total_malloc_size += reqSize;
    newBlock->size = reqSize - sizeof(FreeNode);
    // insert at an ascending address
    if (head < newBlock) {
      newBlock->next = head;
      head = newBlock;
    } else {
      temp = head;
      while (temp->next != NULL && temp->next < newBlock) {
        temp = temp->next;
      }
      newBlock->next = temp->next;
      temp->next = newBlock;
      prev = temp;
    }
    temp = newBlock;
  }

  // split a large block
  if (temp->size > reqSize + THRESHOLD_BLOCK_SIZE) {
    FreeNode* t = temp->next;
    temp->next = (FreeNode*) ((void*) temp + reqSize + 1);
    temp->next->next = t;
    temp->next->size = temp->size - reqSize - sizeof(FreeNode);
    temp->size = reqSize - sizeof(FreeNode);
  }
  if (temp == head) {
    head = temp->next;
  } else {
    prev->next = temp->next;
  }
  temp->next = NULL;
  check_heap();
  return (void*)temp + sizeof(FreeNode);
}

// This function takes a uintptr_t type "size" and returns the
// same number rounded up to the nearest multiple of memory alignment
// size, or itself if it is already a multiple
uintptr_t alignedSize(uintptr_t size) {
  if (size % ALIGNMENT_SIZE) {
    return (size - size % ALIGNMENT_SIZE + ALIGNMENT_SIZE);
  } else {
    return size;
  }
}

/*
 * mem_utils.c - Code for checking the size of the heap as part of memory 
 *     management package
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

# include <stdlib.h>
# include <assert.h>
# include "mem_impl.h"

// Check for possible problems with the free list data structure
void check_heap() {
  FreeNode* temp = head;
  while (temp != NULL && temp->next != NULL) {
    assert(temp < temp->next);
    assert(temp->size > 0 && temp->size >= ALIGNMENT_SIZE);
    assert(((void*) temp + temp->size + sizeof(FreeNode)) -1 <=
            (void*) temp->next);
    assert(((void*) temp + temp->size + sizeof(FreeNode)) -1 !=
            (void*) temp->next);
    temp = temp->next;
  }
}

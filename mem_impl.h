/*
 * mem_impl.h - Header file for function to check heap and other implementation functions
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

#ifndef MEM_IMPL_H
#define MEM_IMPL_H

# include <inttypes.h>

# define ALIGNMENT_SIZE 16

// A node in the freelist storing size of the node and a pointer to the
// next node in the freelist
typedef struct FreeNode {
  struct FreeNode* next;
  uintptr_t size;
}FreeNode;

// Head of the free list
extern FreeNode* head;

void check_heap();

#endif /* ifndef MEM_IMPL_H */

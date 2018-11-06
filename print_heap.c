/*
 * print_heap.c - Outputs information about the current state of the heap to a 
 *     given file.
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

# include <stdio.h>
# include "mem_impl.h"

// Outputs information about the current state of the heap to a given file.
// Takes as an argument:
//     - a FILE pointer type, which will be the output.
void print_heap(FILE * f) {
  FreeNode* temp = head;
  printf("Node address   Size\n");
  while (temp != NULL) {
    fprintf(f, "%-14p %lu\n", (void*) temp, temp->size);
    temp = temp->next;
  }
}

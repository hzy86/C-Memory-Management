/*
 * get_mem_stats.c - stores information on the state of the memory manager.
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

# include <inttypes.h>
# include "mem_impl.h"
# include "mem.h"

// Stores statistics about the state of the memory manager
// Takes as an argument:
//     - an unsigned int pointer representing total amount of storage in bytes
//         aquired by the memory manager to use in satisfying allocation
//         requests
//     - an unsigned int pointer representing total amount of storage in bytes
//         stored on the free list
//     - an unsigned int pointer representing total number of blocks stored on
//         free list
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t*
                   n_free_blocks) {
  *total_size = total_malloc_size;
  FreeNode* temp = head;
  uintptr_t total_storage_size = 0;
  uintptr_t n_freelist_blocks = 0;
  while (temp != NULL) {
    total_storage_size += temp->size;
    n_freelist_blocks++;
    temp = temp->next;
  }
  *total_free = total_storage_size;
  *n_free_blocks = n_freelist_blocks;
}

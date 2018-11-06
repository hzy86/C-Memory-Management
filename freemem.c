/*
 * freemem.c - Code to free dynamically allocate space from the heap
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

# include <stdlib.h>
# include "mem_impl.h"

// Goes through the FreeList looking for adjacent blocks and merges them.
void mergeBlocks() {
  FreeNode* temp = head;
  while (temp != NULL && temp->next != NULL) {
    if ((void*) temp + sizeof(FreeNode) + temp->size ==
          (void*) temp->next - 1) {
      FreeNode* currNode = temp->next;
      temp->next = currNode->next;
      temp->size += currNode->size;
    }
    temp = temp->next;
  }
}

// Frees dynamically allocated space on the heap for alternative use.
// Takes a pointer as an argument
void freemem(void* p) {
  if (p != NULL) {
    check_heap();
    FreeNode* currNode = p - sizeof(FreeNode);
    FreeNode* temp = head;
    if (temp < currNode && temp != NULL) {
      while (temp->next != NULL &&  temp->next < currNode) {
        temp = temp->next;
      }
      if (temp->next != NULL) {
        currNode->next = temp->next;
      }
      temp->next = currNode;
    } else {
      currNode->next = head;
      head = currNode;
    }
    mergeBlocks();
  }
  check_heap();
}

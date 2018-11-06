/*
 * bench.c - Tests the memory management system, with various options
 * CSE 374, HW6, Liam Nagle-Cocco & Ziyi Huang
 */

# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <getopt.h>
# include "mem.h"
# include "mem_impl.h"

// Returns an int pointer of size determined randomly using its arguments.
// Arguments:
//    - int pctget. An integer of size between 0 and 100. Determines the
//      percentage likelihood of a large storage being taken.
//    - int small_limit. The maximum size of a small storage request.
//    - int large_limit. The maximum size of a large storage request.
int* getMemTester(int pctlarge, int small_limit, int large_limit) {
  int getChance = rand() % 100;
  int num = rand()*rand()*rand();
  if (num < 0) {
    num *= (-1);
  }
  if (getChance < pctlarge) {
    num = (num % (large_limit - small_limit)) + small_limit;
  } else {
    num = num % small_limit;
  }
  int* retValue = getmem(num * sizeof(num));
  int loopMax = 16;
  if (num < 16) {
    loopMax = num;
  }
  for (int i = 0; i < loopMax; i++) {
    retValue[i] = 254;
  }
  return(retValue);
}

// Goes through the FreeList and counts the number of blocks. Returns number as
// an integer.
int blockNumber() {
  int count = 1;
  FreeNode* temp = head;
  while (temp->next != NULL) {
    count++;
    temp = temp->next;
  }
  return(count);
}

// Prints tips on correct use of arguments in case of misuse.
void printTips() {
  fprintf(stderr, "Invalid argument supplied.\n\n");
  printf("Arguments must take form:\n");
  char *str = "bench [ntrials] [pctget] [pctlarge] [small_limit] [large_limit] "
    "[random_seed]";
  printf("\t\t%s\n\n", str);
  printf("Options are:\n");
  printf("\t\t-n option is ntrials, the number of tests\n");
  printf("\t\t-g option is pctget, %% getmem tests (0<,<100)\n");
  printf("\t\t-p option is pctlarge, %% large files tests (0<,<100)\n");
  printf("\t\t-s option is small_limit, max size of small file (bytes)\n");
  printf("\t\t-l option is large_limit, max size of large file (bytes)\n");
  printf("\t\t-r option is random_seed, a number which seeds random numbers\n");
  printf("\nPlease try again.\n");
}

// execute a large number of calls to allocate and free blocks of random sizes
// and in random order
int main(int argc, char **argv) {
  double startTime = clock();

  int ntrials = 10000;
  int pctget = 50;
  int pctlarge = 10;
  int small_limit = 200;
  int large_limit = 20000;
  int random_seed = (int) time(NULL);

  int option;
  while ((option = getopt(argc, argv, "n:g:p:s:l:r:")) != -1) {
    switch (option) {
    case 'n':
      ntrials = atoi(optarg);
      break;
    case 'g':
      pctget = atoi(optarg);
      break;
    case 'p':
      pctlarge = atoi(optarg);
      break;
    case 's':
      small_limit = atoi(optarg);
      break;
    case 'l':
      large_limit = atoi(optarg);
      break;
    case 'r':
      random_seed = atoi(optarg);
      break;
    default:
      printTips();
      return EXIT_FAILURE;
    }
  }

  if ((pctget > 100) || (pctlarge > 100)) {
    fprintf(stderr, "Options pctget and pctlarge cannot be greater"
             " than 100.\n");
    return EXIT_FAILURE;
  }

  int **pointers = (int **)malloc(ntrials * sizeof(int*));
  int pointer = 0;

  srand(random_seed);

  for (int counter = 0; counter < ntrials; counter++) {
    int getChance = rand() % 100;
    if (getChance < pctget) {
      pointers[pointer + 2] = getMemTester(pctlarge, small_limit, large_limit);
      pointer++;
    } else {
      if (pointer != 0) {
        int randNum = rand() % pointer;
        if (randNum != 0) {
          freemem(pointers[randNum]);
        }
        if (pointer > 1) {
          pointers[randNum] = pointers[pointer - 1];
          pointers[pointer - 1] = NULL;
        }
        pointer--;
      }
    }

    if ((counter + 1) % (ntrials/10) == 0) {
      uintptr_t* stats = (uintptr_t*) malloc(3 * sizeof(uintptr_t*));
      get_mem_stats(stats, stats + 1, stats + 2);
      printf("At %d%% through tests.\n", 100 * (counter + 1) / ntrials);
      double timeNum = (clock() - startTime)/ CLOCKS_PER_SEC;
      printf("\tCPU time of program is: %.2f seconds\n", timeNum);
      printf("\tNumber of storage aquired is: %lu\n", total_malloc_size);
      uintptr_t numBlocks = (uintptr_t)*(stats + 2);
      printf("\tNumber of free blocks is: %lu\n", numBlocks);
      uintptr_t averageBlock = 0;
      if (numBlocks > 0) {
        averageBlock = (uintptr_t) *(stats + 1) / numBlocks;
      }
      printf("\tAverage storage per block is: %lu\n", averageBlock);
    }
  }
  free(pointers);
  return EXIT_SUCCESS;
}


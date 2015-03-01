#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 256
#define NBYTES 40

// Function headers
void printAll();


/*
 * This is my testing program, it tests for several things:
 * - smallocating the entire size of freelist. this is interesting because
 *    this case rarely occurs, and requires a specific if statement to catch it.
 *    further, special care must be taken when smallocating the entire freelist
 *    as then the freelist becomes null.
 * - freeing all space
 * - freeing an internal node
 * - smallocating a differently size node back in its place
 * - freeing it again to test proper order (increasing mem. addr.) of freelist
 * - smallocating a node of size exactly of the size of a node in freelist
 * - attempting to sfree at a bad address
 * - freeing the last node when size > 1
 * - freeing the first node when size > 1
 * - freeing a single remaining node
 */

int main(void) {
    mem_init(SIZE);
    printf("=========== memory initiation of size %d, ============\n", SIZE);
    printAll();

    char *ptrs[3];

    ptrs[0] = smalloc(SIZE);
    printf("========== smallocating %d bytes to ptrs[0], =========\n", SIZE);
    printAll();


    sfree(ptrs[0]);
    printf("================== sfreeing ptrs[0], ==================\n");
    printAll();
    printf("\n");

    printf("// set up pointers\n");
    ptrs[0] = smalloc(20);
    ptrs[1] = smalloc(NBYTES);
    ptrs[2] = smalloc(60);
    printf("========== smallocating 20 bytes to ptrs[0], ==========\n");
    printf("========== smallocating %d bytes to ptrs[1], ==========\n", NBYTES);
    printf("========== smallocating 60 bytes to ptrs[2], ==========\n");
    printAll();

    printf("// free an internal node\n");
    sfree(ptrs[1]);
    printf("================== sfreeing ptrs[1], ==================\n");
    printAll();

    printf("// smalloc a differently sized node\n");
    ptrs[1] = smalloc(80);
    printf("========== smallocating 80 bytes to ptrs[1], ==========\n");
    printAll();

    printf("// free it again\n");
    printf("// will prompt proper insertion into freelist\n");
    sfree(ptrs[1]);
    printf("================== sfreeing ptrs[1], ==================\n");
    printAll();

    printf("// smalloc back an equally sized node of NBYTES\n");
    printf("// will not split any nodes in freelist\n");
    ptrs[1] = smalloc(NBYTES);
    printf("========== smallocating %d bytes to ptrs[1], ==========\n", NBYTES);
    printAll();

    printf("// attempt to free at an incorrect address\n");
    int i = sfree(ptrs[1]+1);
    // i = -1;
    printf("======= sfreeing at bad address %p, =======\n", ptrs[1]+1);
    printf("================== sfree returned %d ==================\n", i);
    printf("\n");

    printf("// sfree last node in allocated_list, size > 1\n");
    sfree(ptrs[0]);
    printf("================== sfreeing ptrs[0], ==================\n");
    printAll();

    printf("// sfree first node in allocated_list, size > 1\n");
    printf("// will prompt proper insertion into freelist\n");
    sfree(ptrs[1]);
    printf("================== sfreeing ptrs[1], ==================\n");
    printAll();

    printf("// sfree single remaining node in allocated_list\n");
    sfree(ptrs[2]);
    printf("================== sfreeing ptrs[2], ==================\n");
    printAll();

    // clean up memory
    mem_clean();

    return 0;
}

void printAll() {
  printf("    List of free blocks:\n");
  print_free();
  printf("    List of allocated blocks:\n");
  print_allocated();
}

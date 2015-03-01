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
 * test description
 */

int main(void) {
    mem_init(SIZE);
    printf("=========== memory initiation of size %d, ============\n", SIZE);
    printAll();

    char *ptrs[3];

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

    // memory leak here
    printf("// smalloc back an equally sized node of NBYTES\n");
    ptrs[1] = smalloc(NBYTES);
    printf("========== smallocating %d bytes to ptrs[1], ==========\n", NBYTES);
    printAll();


    printf("// free it again\n");
    sfree(ptrs[1]);
    printf("================== sfreeing ptrs[1], ==================\n");
    printAll();

    printf("// attempt to free at an incorrect address\n");
    int i = sfree(ptrs[1]+1);
    printf("======= sfreeing at bad address %p, =======\n", ptrs[1]+1);
    // i = -1;
    printf("================== sfree returned %d ==================\n", i);
    printAll();

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

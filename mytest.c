#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 4096 * 64
#define NBYTES 20

// Function headers
void printAll();


/*
 * test description
 */

int main(void) {
    mem_init(SIZE);
    printf("==== after memory initiation of size %d ====\n", SIZE);
    printAll();

    char *ptrs[2];
    ptrs[0] = smalloc(NBYTES);
    printf("==== after smallocating %d bytes to ptrs[0] ====\n", NBYTES);
    printAll();

    ptrs[1] = smalloc(40);
    printf("==== after smallocating 40 bytes to ptrs[1] ====\n");
    printAll();

    sfree(ptrs[0]);
    printf("==== after sfree'ing ptrs[0] ====\n");
    printAll();



    sfree(ptrs[1]);
    printf("==== after sfree'ing ptrs[1] ====\n");
    printAll();

    /*ptrs[2] = smalloc(60);
    printf("==== after smallocating 60 bytes to ptrs[2] ====\n");
    printAll();*/



    return 0;
}

void printAll() {
  printf("    List of free blocks:\n");
  print_free();
  printf("    List of allocated blocks:\n");
  print_allocated();
}

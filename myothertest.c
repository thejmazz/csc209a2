#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 42

// Function headers
void printAll();


/*
 * test description
 */

int main(void) {
    mem_init(SIZE);
    printf("==== memory initiation of size %d, ====\n", SIZE);
    printAll();

    char *ptrs[2];

    ptrs[0] = smalloc(SIZE);
    printf("==== smallocating %d bytes to ptrs[0], ====\n", SIZE);
    printAll();

    sfree(ptrs[0]);
    printf("==== sfreeing ptrs[0], ====\n");
    printAll();

    ptrs[0] = smalloc(22);
    printf("==== smallocating 22 bytes to ptrs[0], ====\n");
    printAll();

    ptrs[1] = smalloc(42);
    printf("==== smallocating 42 bytes to ptrs[1], ====\n");
    printAll();

    ptrs[1] = smalloc(20);
    printf("==== smallocating 20 bytes to ptrs[1], ====\n");
    printAll();

    sfree(ptrs[0]);
    printf("==== sfreeing ptrs[0], ====\n");
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

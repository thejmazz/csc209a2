#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"

// Function headers
void *createNode(struct block *curr, unsigned int nbytes);
void mem_clean_help(struct block *head);

/* The starting address of the memory region that is reserved by mem_init */
void *mem;

/* A linked list of struct blocks that identify the portions of the memory
 * region that are free. Blocks in this list are stored in increasing address
 * order.
 */
struct block *freelist;

/* A linked list of struct blocks that identify portions of memory that have
 * been reserved by calls to smalloc. When a block is allocated it is placed
 * at the front of this list, so the list is unordered.
 */
struct block *allocated_list;


void *smalloc(unsigned int nbytes) {
    // set current block to head of freelist
    struct block *curr = freelist;
    // initialize a previous block as well
    struct block *prev = curr;

    // search freelist for a node with node.size >= nbytes
    while (curr->size < nbytes) {
      prev = curr;
      curr = curr->next;
    }

    // exit if no block with enough space was found
    // or trying to smalloc 0 bytes
    if (curr == NULL || nbytes == 0) {
      return NULL;
    }

    if (curr->size == nbytes) {
      // append temp to head of allocated_list
      allocated_list = createNode(curr, nbytes);

      // no need to split block from freelist
      // fix references in freelist
      prev->next = curr->next;

    } else if (curr->size > nbytes) {
      // append temp to head of allocated_list
      allocated_list = createNode(curr, nbytes);

      // split block from freelist
      // change address and size of current node in freelist
      curr->addr = curr->addr + nbytes;
      curr->size = curr->size - nbytes;
    }
    return allocated_list->addr;
}


void *createNode(struct block *curr, unsigned int nbytes) {
  if (allocated_list->size == -1) {
    // No memory has been smallocated before,
    // modify existing node in allocated_list
    allocated_list->addr = curr->addr;
    allocated_list->size = nbytes;
    allocated_list->next = NULL;

    return allocated_list;

  } else {
    // Create a new node that will be appended
    // to the front of allocated_list
    struct block *temp = malloc(sizeof(struct block));

    temp->addr = curr->addr;
    temp->size = nbytes;
    temp->next = allocated_list;

    return temp;
  }
}


int sfree(void *addr) {
  // set toFree node to head of allocated_list
  struct block *toFree = allocated_list;
  // initialize a previous block as well
  struct block *prev = toFree;

  // search allocated_list for a node
  // with the correct address
  while (toFree->addr != addr && toFree != NULL) {
    prev = toFree;
    toFree = toFree->next;
  }

  if (!toFree) {
    // node with correct address was not found
    return -1;
  }

  // Fix references in allocated_list
  prev->next = toFree->next;

  // Move toFree node to front of freelist
  toFree->next = freelist;
  freelist = toFree;

  return 0;
}


/* Initialize the memory space used by smalloc,
 * freelist, and allocated_list
 * Note:  mmap is a system call that has a wide variety of uses.  In our
 * case we are using it to allocate a large region of memory.
 * - mmap returns a pointer to the allocated memory
 * Arguments:
 * - NULL: a suggestion for where to place the memory. We will let the
 *         system decide where to place the memory.
 * - PROT_READ | PROT_WRITE: we will use the memory for both reading
 *         and writing.
 * - MAP_PRIVATE | MAP_ANON: the memory is just for this process, and
 *         is not associated with a file.
 * - -1: because this memory is not associated with a file, the file
 *         descriptor argument is set to -1
 * - 0: only used if the address space is associated with a file.
 */
void mem_init(int size) {
    // assign mem a pointer to the start of allocated memory
    mem = mmap(NULL, size,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(mem == MAP_FAILED) {
         perror("mmap");
         exit(1);
    }


    // initialize freelist
    freelist = malloc(sizeof(struct block));
    // starting address as given by mmap
    freelist->addr = mem;
    // size as given by param size
    freelist->size = size;
    // single-node linked-list, set next to NULL
    freelist->next = NULL;


    // initialize an empty allocated_list
    allocated_list = malloc(sizeof(struct block));
    allocated_list->addr = NULL;
    allocated_list->size = -1;
    allocated_list->next = NULL;
}

void mem_clean(){
  mem_clean_help(allocated_list);
  mem_clean_help(freelist);
}

void mem_clean_help(struct block *head) {
  // set current block to head of linked list
  struct block *curr = head;
  // initialize a temp block pointer
  struct block *temp;

  // free all nodes in the linked list
  while (curr != NULL) {
    temp = curr;
    free(curr);
    curr = temp->next;
  }
}

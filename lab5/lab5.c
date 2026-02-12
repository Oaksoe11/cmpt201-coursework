#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *current = free_list_ptr;

  while (current != NULL) {
    if (current->size >= size) {
      return current->id;
    }
    current = current->next;
  }

  return -1; // not found
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *current = free_list_ptr;
  int best_fit_id = -1;
  uint64_t smallest_diff = UINT64_MAX;

  while (current != NULL) {
    if (current->size >= size) {
      uint64_t diff = current->size - size;
      if (diff < smallest_diff) {
        smallest_diff = diff;
        best_fit_id = current->id;
      }
    }
    current = current->next;
  }

  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *current = free_list_ptr;
  int worst_fit_id = -1;
  uint64_t largest_size = 0;

  while (current != NULL) {
    if (current->size >= size) {
      if (current->size > largest_size) {
        largest_size = current->size;
        worst_fit_id = current->id;
      }
    }
    current = current->next;
  }

  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  return 0;
}

/*
====================================================
Part 2: Coalescing Contiguous Free Blocks (Pseudo-Code)
====================================================

Assumption:
- The free list is already sorted by memory address.
- Only ONE block has just been freed.
- Existing free blocks are already coalesced.

Pseudo-Code:

function coalesce(new_block, free_list_head):

    prev = NULL
    current = free_list_head

    // Step 1: Find correct insertion position (by address order)
    while current != NULL AND current address < new_block address:
        prev = current
        current = current->next

    // Step 2: Insert new_block between prev and current
    new_block->next = current
    if prev == NULL:
        free_list_head = new_block
    else:
        prev->next = new_block

    // Step 3: Merge with next block if contiguous
    if current != NULL AND
       (address of new_block + new_block->size == address of current):

        new_block->size += current->size
        new_block->next = current->next

    // Step 4: Merge with previous block if contiguous
    if prev != NULL AND
       (address of prev + prev->size == address of new_block):

        prev->size += new_block->size
        prev->next = new_block->next

        new_block = prev   // merged into prev

    return free_list_head

----------------------------------------------------

Important Cases Covered:


*/

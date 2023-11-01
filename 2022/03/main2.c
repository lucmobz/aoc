#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RUCKSACK_SIZE 64
#define ITEM_COUNT 52
#define ELF_COUNT 3

typedef uint64_t u64;

static char rucksack[RUCKSACK_SIZE] = {0};

int main(void) {

  u64 item_lists[ELF_COUNT][ITEM_COUNT] = {0};
  u64 elf_id = 0;
  u64 sum_priorities = 0;

  while (fgets(rucksack, RUCKSACK_SIZE, stdin)) {
    // Strip newline
    size_t count = strlen(rucksack) - 1;
    assert(count % 2 == 0);

    // Fill the item list for each elf by parsing the rucksack
    for (size_t i = 0; i < count; ++i) {
      char c = rucksack[i];
      size_t item_id = isupper(c) ? c - 'A' + 26 : c - 'a';
      ++item_lists[elf_id % 3][item_id];
    }

    ++elf_id;

    // Process item lists every 3 elves
    if (elf_id % 3 == 0) {
      for (size_t item_id = 0; item_id < ITEM_COUNT; ++item_id) {
        // This should happen only for the badge item
        if (item_lists[0][item_id] && item_lists[1][item_id] &&
            item_lists[2][item_id])
          sum_priorities += item_id + 1;
      }

      // Reset item lists
      memset(item_lists, 0, ELF_COUNT * ITEM_COUNT * sizeof(u64));
    }
  }

  printf("Part 2: %lu\n", sum_priorities);
}

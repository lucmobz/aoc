#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Get how many characters is the max rucksack
// while IFS= read -r line; do echo "$line" | wc -c; done < input.txt |
// sort | tail -1
#define RUCKSACK_SIZE 64
#define ITEM_COUNT 52
#define COMPARMENT_COUNT 2

typedef uint64_t u64;

static char rucksack[RUCKSACK_SIZE] = {0};

int main(void) {

  u64 sum_priorities = 0;

  while (fgets(rucksack, RUCKSACK_SIZE, stdin)) {
    u64 item_list[COMPARMENT_COUNT][ITEM_COUNT] = {0};

    // Strip newline
    size_t count = strlen(rucksack) - 1;
    assert(count % 2 == 0);

    for (size_t i = 0; i < count; ++i) {
      char c = rucksack[i];
      size_t item_id = isupper(c) ? c - 'A' + 26 : c - 'a';
      size_t compartment_id = i < count / 2;
      ++item_list[compartment_id][item_id];
    }

    for (size_t item_id = 0; item_id < ITEM_COUNT; ++item_id) {
      if (item_list[0][item_id] && item_list[1][item_id])
        sum_priorities += item_id + 1;
    }
  }

  printf("Part 1:\n%lu\n", sum_priorities);
}

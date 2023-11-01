#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 64

static char line[LINE_SIZE];

// clang-format off
static char stacks[9][128] = {
  {'D', 'T', 'W', 'F', 'J', 'S', 'H', 'N'},
  {'H', 'R', 'P', 'Q', 'T', 'N', 'B', 'G'},
  {'L', 'Q', 'V'},
  {'N', 'B', 'S', 'W', 'R', 'Q'},
  {'N', 'D', 'F', 'T', 'V', 'M', 'B'},
  {'M', 'D', 'B', 'V', 'H', 'T', 'R'},
  {'D', 'B', 'Q', 'J'},
  {'D', 'N', 'J', 'V', 'R', 'Z', 'H', 'Q'},
  {'B', 'N', 'H', 'M', 'S'}};
// clang-format on

int main(void) {

  // Skip stack parsing as it is hardcoded
  while (fgets(line, LINE_SIZE, stdin)) {
    if (line[0] == '\n')
      break;
  }

  // Parse instructions
  while (fgets(line, LINE_SIZE, stdin)) {
    char *ptr = line;

    unsigned crate_count = strtoul(ptr + 5, &ptr, 10);
    size_t from = strtoul(ptr + 6, &ptr, 10) - 1;
    size_t to = strtoul(ptr + 4, &ptr, 10) - 1;

    while (crate_count-- > 0) {
      size_t to_len = strlen(stacks[to]);
      size_t from_len = strlen(stacks[from]);
      stacks[to][to_len] = stacks[from][from_len - 1];
      stacks[from][from_len - 1] = 0;
    }
  }

  char msg[10] = {0};
  for (size_t i = 0; i < sizeof(stacks) / sizeof(stacks[0]); ++i) {
    msg[i] = stacks[i][strlen(stacks[i]) - 1];
  }

  printf("Part 1: %s\n", msg);
}

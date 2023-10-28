#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 32

// Automatically init to zero
static char line[LINE_SIZE];

int main(void) {
  unsigned long long max_calories = 0;
  unsigned long long current_calories = 0;

  while (fgets(line, LINE_SIZE, stdin) != NULL) {
    if (line[0] == '\n') {
      if (current_calories > max_calories)
        max_calories = current_calories;
      current_calories = 0;
    } else {
      current_calories += strtoll(line, NULL, 10);
    }
  }

  printf("Part 1:\n%llu\n", max_calories);
}

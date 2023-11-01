#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 32
#define ELF_COUNT 3

typedef unsigned long long ull_t;

static char line[LINE_SIZE] = {0};
static ull_t max_calories[ELF_COUNT] = {0};
static ull_t current_calories = 0;

static void copy_backward(ull_t *dst, const ull_t *src, size_t count) {
  while (count-- > 0)
    dst[count] = src[count];
}

static void shift_right() {
  // try to find if there is a maximum
  for (size_t i = 0; i < ELF_COUNT; ++i) {
    if (current_calories > max_calories[i]) {
      // shift all to the right, chopping the last element
      copy_backward(max_calories + (i + 1), max_calories + i,
                    ELF_COUNT - i - 1);
      max_calories[i] = current_calories;
      break;
    }
  }
  // reset status and prepare to read new elf
  current_calories = 0;
}

int main(void) {

  while (fgets(line, LINE_SIZE, stdin) != NULL) {
    if (line[0] == '\n') {
      shift_right();
    } else {
      current_calories += strtoll(line, NULL, 10);
    }
  }
  shift_right();

  ull_t sum_max_calories = 0;
  for (size_t i = 0; i < ELF_COUNT; ++i)
    sum_max_calories += max_calories[i];

  printf("Part 2: %llu\n", sum_max_calories);
}

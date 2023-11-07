#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 16
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 6

typedef int64_t i64;
typedef uint64_t u64;

static i64 x_register = 1;
static u64 clock = 1;
static i64 signal_strength = 0;
static i64 signal_strength_sum = 0;
static char screen[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};

void cycle_clock() {
  signal_strength = clock * x_register;

  if (clock == 20)
    signal_strength_sum += signal_strength;
  else if (clock > 20 && clock <= 220 && (clock - 20) % SCREEN_WIDTH == 0)
    signal_strength_sum += signal_strength;

  assert(x_register <= SCREEN_WIDTH && x_register >= -1);
  assert(clock <= SCREEN_HEIGHT * SCREEN_WIDTH);

  u64 drawn_pixel = (clock - 1) % SCREEN_WIDTH;
  if (x_register - 1 <= (i64)drawn_pixel && (i64)drawn_pixel <= x_register + 1)
    ((char *)screen)[clock - 1] = '#';
  else
    ((char *)screen)[clock - 1] = '.';

  ++clock;
}

void dump_screen() {
  for (size_t y = 0; y < SCREEN_HEIGHT; ++y) {
    for (size_t x = 0; x < SCREEN_WIDTH; ++x) {
      putchar(screen[y][x]);
    }
    putchar('\n');
  }
}

int main(void) {
  char line[LINE_SIZE] = {0};

  while (fgets(line, LINE_SIZE, stdin)) {
    switch (line[0]) {
    case 'a': {
      cycle_clock();
      cycle_clock();
      x_register += strtoll(line + 5, NULL, 10);
    } break;
    case 'n':
      cycle_clock();
      break;
    }
  }

  printf("Part 1: %ld\n", signal_strength_sum);
  puts("Part 2:");
  dump_screen();
}

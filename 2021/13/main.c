#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAP 2048

static char grid[CAP][CAP] = {0};
static int xmax = 0;
static int ymax = 0;

void dump_grid() {
  for (int y = 0; y <= ymax; ++y) {
    for (int x = 0; x <= xmax; ++x) {
      if (grid[y][x])
        putchar('#');
      else
        putchar('.');
    }
    putchar('\n');
  }
}

void fold_grid(char axis, int fold) {
  if (axis == 'x') {
    for (int y = 0; y <= ymax; ++y)
      for (int x = 2 * fold - xmax; x < fold; ++x)
        if (grid[y][2 * fold - x]) grid[y][x] = 1;
    xmax = fold - 1;
  } else if (axis == 'y') {
    for (int x = 0; x <= xmax; ++x)
      for (int y = 2 * fold - ymax; y < fold; ++y)
        if (grid[2 * fold - y][x]) grid[y][x] = 1;
    ymax = fold - 1;
  } else {
    assert(false);
  }
}

int count_grid() {
  int count = 0;
  for (int y = 0; y <= ymax; ++y)
    for (int x = 0; x <= xmax; ++x) count += grid[y][x];
  return count;
}

int main(void) {
  char line[64];

  while (fgets(line, 64, stdin)) {
    if (isdigit(line[0])) {
      char* p = line;

      int x = strtol(p, &p, 10);
      int y = strtol(p + 1, NULL, 10);
      grid[y][x] = 1;

      xmax = x > xmax ? x : xmax;
      ymax = y > ymax ? y : ymax;
    } else if (line[0] == 'f') {
      char* p = line + 11;
      char axis = p[0];
      int fold = strtol(p + 2, NULL, 10);

      fold_grid(axis, fold);
    }
  }

  printf("Count: %d\n", count_grid());
  dump_grid();
}
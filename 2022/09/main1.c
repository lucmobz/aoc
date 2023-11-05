#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 8
#define SIGN(x) ((x) > 0 ? 1 : (x) == 0 ? 0 : -1)
#define GRID_SIZE 1024
#ifndef ROPE_SIZE
#define ROPE_SIZE 10
#endif
#ifndef PART
#define PART 2
#endif

int main(void) {
  char line[LINE_SIZE] = {0};

  int rope[ROPE_SIZE][2] = {0};
  // Keep track of tail positions (2D grid)
  bool position_grid[GRID_SIZE][GRID_SIZE] = {0};

  int x_tail = rope[ROPE_SIZE - 1][0];
  int y_tail = rope[ROPE_SIZE - 1][1];
  int x0 = GRID_SIZE / 2;
  int y0 = GRID_SIZE / 2;
  position_grid[x0 + x_tail][y0 + y_tail] = true;

  while (fgets(line, LINE_SIZE, stdin)) {
    unsigned long long count = strtoull(line + 2, NULL, 10);

    while (count-- > 0) {
      // Move head
      switch (line[0]) {
      case 'R':
        ++rope[0][0];
        break;
      case 'L':
        --rope[0][0];
        break;
      case 'U':
        ++rope[0][1];
        break;
      case 'D':
        --rope[0][1];
        break;
      }

      for (size_t i = 1; i < ROPE_SIZE; ++i) {
        int dx = rope[i - 1][0] - rope[i][0];
        int dy = rope[i - 1][1] - rope[i][1];
        if (abs(dx) > 1 || abs(dy) > 1) {
          rope[i][0] += SIGN(dx);
          rope[i][1] += SIGN(dy);
        }
      }

      int x_tail = rope[ROPE_SIZE - 1][0];
      int y_tail = rope[ROPE_SIZE - 1][1];
      assert(x_tail < GRID_SIZE / 2);
      assert(y_tail < GRID_SIZE / 2);

      position_grid[x0 + x_tail][y0 + y_tail] = true;
    }
  }

  unsigned long long count = 0;
  for (size_t i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
    if (((bool *)position_grid)[i])
      ++count;
  }

  printf("Part %d: %llu\n", PART, count);
}

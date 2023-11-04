#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_SIZE 128
#ifndef GRID_SIZE
#define GRID_SIZE 99
#endif

static char line[LINE_SIZE] = {0};
static char grid[GRID_SIZE][GRID_SIZE] = {0};
static char visible[GRID_SIZE][GRID_SIZE] = {0};

void dump_visibility() {
  for (size_t y = 0; y < GRID_SIZE; ++y) {
    for (size_t x = 0; x < GRID_SIZE; ++x) {
      if (visible[y][x])
        putchar('#');
      else
        putchar('.');
    }
    putchar('\n');
  }
}

unsigned long long compute_directional_score(size_t x, size_t y, int dx,
                                             int dy) {
  char max_height = grid[y][x];
  unsigned long long score = 0;

  // Already know that the first point is valid
  do {
    x += dx;
    y += dy;
    ++score;
  } while (x > 0 && y > 0 && x < GRID_SIZE - 1 && y < GRID_SIZE - 1 &&
           grid[y][x] < max_height);
  // Already know that the first point is valid

  return score;
}

unsigned long long compute_viewing_score(size_t x0, size_t y0) {
  if (x0 <= 0 || x0 >= GRID_SIZE - 1 || y0 <= 0 || y0 >= GRID_SIZE - 1)
    return 0;

  unsigned long long score = 1;
  score *= compute_directional_score(x0, y0, 1, 0);
  score *= compute_directional_score(x0, y0, -1, 0);
  score *= compute_directional_score(x0, y0, 0, 1);
  score *= compute_directional_score(x0, y0, 0, -1);
  return score;
}

int main(void) {

  size_t y = 0;
  while (fgets(line, LINE_SIZE, stdin)) {
    strncpy(grid[y++], line, GRID_SIZE);
  }

  for (size_t i = 0; i < GRID_SIZE; ++i) {
    visible[0][i] = visible[GRID_SIZE - 1][i] = true;
    visible[i][0] = visible[i][GRID_SIZE - 1] = true;
  }

  // dump_visibility();

  for (size_t y = 1; y < GRID_SIZE - 1; ++y) {
    char curr_max_left = grid[y][0];
    char curr_max_right = grid[y][GRID_SIZE - 1];

    for (size_t x = 1; x < GRID_SIZE - 1; ++x) {
#if 1
      if (grid[y][x] > curr_max_left) {
        curr_max_left = grid[y][x];
        visible[y][x] = true;
      }
#endif
#if 1
      if (grid[y][GRID_SIZE - 1 - x] > curr_max_right) {
        curr_max_right = grid[y][GRID_SIZE - 1 - x];
        visible[y][GRID_SIZE - 1 - x] = true;
      }
#endif
    }
  }

  for (size_t x = 1; x < GRID_SIZE - 1; ++x) {
    char curr_max_top = grid[0][x];
    char curr_max_bottom = grid[GRID_SIZE - 1][x];

    for (size_t y = 1; y < GRID_SIZE - 1; ++y) {
#if 1
      if (grid[y][x] > curr_max_top) {
        curr_max_top = grid[y][x];
        visible[y][x] = true;
      }
#endif
#if 1
      if (grid[GRID_SIZE - 1 - y][x] > curr_max_bottom) {
        curr_max_bottom = grid[GRID_SIZE - 1 - y][x];
        visible[GRID_SIZE - 1 - y][x] = true;
      }
#endif
    }
  }

  // dump_visibility();

  unsigned long long visible_count = 0;
  for (size_t y = 0; y < GRID_SIZE; ++y) {
    for (size_t x = 0; x < GRID_SIZE; ++x) {
      if (visible[y][x])
        ++visible_count;
    }
  }

  printf("Part 1: %llu\n", visible_count);

  // Edge trees score will be 0
  unsigned long long max_score = 0;
  for (size_t y = 1; y < GRID_SIZE - 1; ++y) {
    for (size_t x = 1; x < GRID_SIZE - 1; ++x) {
      unsigned long long score = compute_viewing_score(x, y);
      if (score > max_score)
        max_score = score;
    }
  }

  printf("Part 2: %llu\n", max_score);
}

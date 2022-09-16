#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int grid[10][10] = {0};

void read_grid() {
  int* ptr = (int*)grid;
  char c = 0;
  while ((c = getchar()) != EOF) {
    if (c != '\n') {
      *ptr++ = c - '0';
    }
  }
}

void dump_grid() {
  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 10; ++x) {
      printf("%d", grid[y][x]);
    }
    putchar('\n');
  }
}

static int dirs[8][2] = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                         {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

void pulse_grid(int x0, int y0) {
  for (size_t i = 0; i < 8; ++i) {
    int x = x0 + dirs[i][0];
    int y = y0 + dirs[i][1];

    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
      ++grid[y][x];
    }
  }
}

int update_grid() {
  int* ptr = (int*)grid;

  for (size_t i = 0; i < 100; ++i) ptr[i] += 1;

  bool pulsed[10][10] = {0};
  int count = 0;
  bool any_pulse;
  do {
    any_pulse = false;
    for (int y = 0; y < 10; ++y) {
      for (int x = 0; x < 10; ++x) {
        if (grid[y][x] > 9 && !pulsed[y][x]) {
          any_pulse = true;
          pulsed[y][x] = true;
          ++count;
          pulse_grid(x, y);
        }
      }
    }
  } while (any_pulse);

  for (size_t i = 0; i < 100; ++i)
    if (ptr[i] > 9) ptr[i] = 0;

  return count;
}

int main(void) {
  read_grid();
  int count = 0;
  for (int i = 0; true; ++i) {
    count += update_grid();
    int* ptr = (int*)grid;
    while (ptr < (int*)grid + 100 && *ptr++ == 0) ;
    if (ptr == (int*)grid + 100) {
      printf("%d\n", i + 1);
      break;
    }
  }
  dump_grid();
  printf("%d\n", count);
}
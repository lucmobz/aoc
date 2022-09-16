#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) < (b) ? (b) : (a))
#define LINE_SZ 256

static char line[LINE_SZ];

void dump_grid(const int* grid, int nx, int ny) {
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      int num = grid[x + y * nx];
      if (num > 0)
        printf(" %2d ", num);
      else
        printf(" %2c ", '.');
    }
    printf("\n");
  }
}

bool sort_pair(int* a, int* b) {
  if (*a > *b) {
    int tmp = *b;
    *b = *a;
    *a = tmp;

    return true;
  }

  return false;
}

bool read_grid_range(const char* filepath, int* nx, int* ny) {
  FILE* file = fopen(filepath, "rb");

  if (file == NULL) {
    fprintf(stderr, "ERROR: could not open %s: %s\n", filepath,
            strerror(errno));
    return false;
  }

  *nx = -1;
  *ny = -1;

  int x0, x1, y0, y1;

  while (fscanf(file, "%d,%d -> %d,%d", &x0, &y0, &x1, &y1) == 4) {
    *nx = max(*nx, max(x0, x1));
    *ny = max(*ny, max(y0, y1));
  }

  *nx += 1;
  *ny += 1;

  fclose(file);

  return true;
}

size_t count_grid(const int* grid, int nx, int ny) {
  size_t count = 0;

  for (int y = 0; y < ny; ++y)
    for (int x = 0; x < nx; ++x)
      if (grid[x + y * nx] >= 2) ++count;

  return count;
}

static inline int sign(int x) {
  if (x > 0) return 1;
  else if (x < 0) return -1;
  else return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file.txt>\n", argv[0]);
    return 1;
  }

  int* grid;
  int nx, ny;

  if (!read_grid_range(argv[1], &nx, &ny)) {
    fprintf(stderr, "ERROR: could not read grid range\n");
    return 1;
  }

  printf("Grid range: %d, %d\n", nx, ny);

  grid = malloc(sizeof(int64_t) * nx * ny);
  memset(grid, 0, sizeof(int64_t) * nx * ny);

  FILE* file = fopen(argv[1], "rb");

  while (fgets(line, LINE_SZ, file)) {
    int x0, y0;
    int x1, y1;
    sscanf(line, "%d,%d -> %d,%d", &x0, &y0, &x1, &y1);

    int x = x0;
    int y = y0;
    int dx = sign(x1 - x0);
    int dy = sign(y1 - y0);

    while (x != x1 || y != y1) {
      ++grid[x + y * nx];
      x += dx;
      y += dy;
    }
    ++grid[x + y * nx];
/*
    bool switch_diagonal = sort_pair(&x0, &x1) ^ sort_pair(&y0, &y1);

    if (x0 == x1) {  // vertical
      for (int y = y0; y <= y1; ++y) ++grid[x0 + y * nx];
    } else if (y0 == y1) {  // horizontal
      for (int x = x0; x <= x1; ++x) ++grid[x + y0 * nx];
    } else {  // diagonal
      int y = switch_diagonal ? y1 : y0;

      for (int x = x0; x <= x1; ++x) {
        ++grid[x + y * nx];
        switch_diagonal ? (--y) : (++y);
      }
    }
*/
  }

  fclose(file);

  printf("%zu\n", count_grid(grid, nx, ny));
}

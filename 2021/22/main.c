#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Cube {
  int xmin, xmax, ymin, ymax, zmin, zmax;
  bool status;
} Cube;

int less(const void* a, const void* b) {
  if (*(const int*)a < *(const int*)b) return -1;
  if (*(const int*)a > *(const int*)b) return 1;
  return 0;
}

void sort(int* xs, size_t sz) { qsort(xs, sz, sizeof(int), less); }

size_t find(int* xs, size_t sz, int x) {
  for (size_t i = 0; i < sz; ++i)
    if (xs[i] == x) return i;
  return sz;
}

int main(void) {
  Cube cs[512];
  int xs[1024];
  int ys[1024];
  int zs[1024];
  size_t sz = 0;

  char line[256];
  while (fgets(line, 256, stdin)) {
    int xmin, xmax, ymin, ymax, zmin, zmax;
    sscanf(line, "%*s x=%d..%d,y=%d..%d,z=%d..%d", &xmin, &xmax, &ymin, &ymax,
           &zmin, &zmax);
    bool status = line[1] == 'n' ? true : false;

    /* Part 1
    xmin = xmin < -50 ? -50 : xmin;
    xmax = xmax > 50 ? 50 : xmax;
    ymin = ymin < -50 ? -50 : ymin;
    ymax = ymax > 50 ? 50 : ymax;
    zmin = zmin < -50 ? -50 : zmin;
    zmax = zmax > 50 ? 50 : zmax;
    */

    ++xmax;
    ++ymax;
    ++zmax;

    Cube c = {xmin, xmax, ymin, ymax, zmin, zmax, status};
    assert(sz <= 512);
    cs[sz] = c;
    xs[sz * 2] = xmin;
    xs[sz * 2 + 1] = xmax;
    ys[sz * 2] = ymin;
    ys[sz * 2 + 1] = ymax;
    zs[sz * 2] = zmin;
    zs[sz * 2 + 1] = zmax;
    ++sz;
  }

  size_t gsz = sz * 2;
  bool (*grid)[1024][1024] = malloc(1024 * sizeof(*grid));

  sort(xs, gsz);
  sort(ys, gsz);
  sort(zs, gsz);

  for (size_t i = 0; i < sz; ++i) {
    size_t xmin = find(xs, gsz, cs[i].xmin);
    size_t xmax = find(xs, gsz, cs[i].xmax);
    size_t ymin = find(ys, gsz, cs[i].ymin);
    size_t ymax = find(ys, gsz, cs[i].ymax);
    size_t zmin = find(zs, gsz, cs[i].zmin);
    size_t zmax = find(zs, gsz, cs[i].zmax);

    for (size_t x = xmin; x < xmax; ++x)
      for (size_t y = ymin; y < ymax; ++y)
        for (size_t z = zmin; z < zmax; ++z) grid[x][y][z] = cs[i].status;
  }

  uint64_t count = 0;
  for (size_t x = 0; x < gsz - 1; ++x)
    for (size_t y = 0; y < gsz - 1; ++y)
      for (size_t z = 0; z < gsz - 1; ++z)
        count += (uint64_t)grid[x][y][z] * (xs[x + 1] - xs[x]) * (ys[y + 1] - ys[y]) *
                 (zs[z + 1] - zs[z]);

  printf("Part 2: %lu\n", count);

  free(grid);
}
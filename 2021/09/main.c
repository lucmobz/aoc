#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Data {
  int z;
  int b;
} Data;

static Data* grid = NULL;
static size_t sz = 0;
static size_t cap = 0;
static int nx = 0;
static int ny = 0;

static const int dir[][2] {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

Data* get(int x, int y) {
  if (x < 0 || x >= nx || y < 0 || y >= ny)
    return NULL;
  else
    return &grid[x + y * nx];
}

void grid_reserve(size_t new_sz) {
  if (cap >= new_sz) return;
  if (cap == 0) cap = 1;
  while (cap < new_sz) cap *= 2;
  grid = realloc(grid, sizeof(*grid) * cap);
}

void dump_grid() {
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      printf("%d", get(x, y)->z);
    }
    printf("\n");
  }
}

#define QSZ (1 << 16)
int find_basin(int x0, int y0) {
  static int b = 1;
  int bsz = 0;

  if (get(x0, y0)->z >= 9 || get(x0, y0)->b > 0) return 0;

  int q[QSZ * 2];
  int qft = 0;
  int qsz = 0;

  q[qsz * 2] = x0;
  q[qsz * 2 + 1] = y0;
  ++qsz;
  get(x0, y0)->b = b;
  ++bsz;

  while (qft != qsz) {
    int x = q[qft * 2];
    int y = q[qft * 2 + 1];
    int z = get(x, y)->z;
    ++qft;

    Data* data[4];
    int xy[8] = {x + 1, y, x - 1, y, x, y + 1, x, y - 1};
    data[0] = get(x + 1, y);
    data[1] = get(x - 1, y);
    data[2] = get(x, y + 1);
    data[3] = get(x, y - 1);

    for (int i = 0; i < 4; ++i) {
      Data* d = data[i];
      int xx = xy[2 * i];
      int yy = xy[2 * i + 1];

      if (d && d->b == 0 && d->z < 9 && d->z > z) {
        q[qsz * 2] = xx;
        q[qsz * 2 + 1] = yy;
        ++qsz;
        get(xx, yy)->b = b;
        ++bsz;
      }
    }
  }

  ++b;
  return bsz;
}

int main(void) {
  int c = 0;

  size_t i = 0;
  while (true) {
    c = getchar();

    if (c == '\n') {
      if (nx == 0) nx = i;
      ++ny;
      i = 0;

      if ((c = getchar()) == EOF)
        break;
      else
        ungetc(c, stdin);
    } else if (c == EOF) {
      ++ny;
      break;
    } else {
      grid_reserve(sz + 1);
      grid[sz++].z = c - '0';
      ++i;
    }
  }

  long long unsigned risk = 0;
  int bsz0 = 0;
  int bsz1 = 0;
  int bsz2 = 0;

  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      int z = get(x, y)->z;
      if (z == 9) continue;

      int zxp = x < nx - 1 ? get(x + 1, y)->z : 10;
      int zxm = x > 0 ? get(x - 1, y)->z : 10;
      int zyp = y < ny - 1 ? get(x, y + 1)->z : 10;
      int zym = y > 0 ? get(x, y - 1)->z : 10;

      if (z < zxp && z < zxm && z < zym && z < zyp) {
        risk += 1 + z;
        int bsz = find_basin(x, y);

        if (bsz > bsz0) {
          bsz2 = bsz1;
          bsz1 = bsz0;
          bsz0 = bsz;
        } else if (bsz > bsz1) {
          bsz2 = bsz1;
          bsz1 = bsz;
        } else if (bsz > bsz2) {
          bsz2 = bsz;
        }
      }
    }
  }

  printf("Risk: %llu\n", risk);
  printf("Answer: %d\n", bsz0 * bsz1 * bsz2);
}
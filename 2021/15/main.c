#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SZ 512 

static int64_t grid[SZ][SZ] = {0};
static int64_t nx = 0;
static int64_t ny = 0;

static bool vis[SZ][SZ] = {0};
static int64_t dist[SZ][SZ] = {0};

static int64_t dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void dump_grid() {
  for (int64_t y = 0; y < ny; ++y) {
    for (int64_t x = 0; x < nx; ++x) printf("%lu", grid[y][x]);
    putchar('\n');
  }
}

void expand_grid() {
  for (int64_t i = 0; i < 5; ++i) {
    for (int64_t j = 0; j < 5; ++j) {
      if (j > 0 || i > 0) {
        for (int64_t y = 0; y < ny; ++y) {
          for (int64_t x = 0; x < nx; ++x) {
            int64_t n = grid[y][x] + i + j;
            while (n > 9) n -= 9;

            grid[y + i * ny][x + j * nx] = n;
          }
        }
      }
    }
  }
  nx *= 5;
  ny *= 5;
}

int main(void) {
  char c = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      nx = 0;
      ++ny;
    } else {
      grid[ny][nx++] = c - '0';
    }
  }
  ++ny;

  expand_grid();
  printf("%ldx%ld\n", ny, nx);
//  dump_grid();
  
  for (int64_t y = 0; y < ny; ++y) {
    for (int64_t x = 0; x < nx; ++x) {
      dist[y][x] = INT64_MAX;
    }
  }
  dist[0][0] = 0;

  int64_t x0 = 0;
  int64_t y0 = 0;

  while (true) {
    for (int64_t i = 0; i < 4; ++i) {
      int64_t x = x0 + dirs[i][0];
      int64_t y = y0 + dirs[i][1];

      if (x >= 0 && x < nx && y >= 0 && y < ny && vis[y][x] == false) {
        int64_t d = grid[y][x] + dist[y0][x0];
        if (d < dist[y][x]) dist[y][x] = d;
      }
    }

    vis[y0][x0] = true;

    if (vis[ny - 1][nx - 1] == true) break;

    int64_t d = INT64_MAX;
    for (int64_t y = 0; y < ny; ++y) {
      for (int64_t x = 0; x < nx; ++x) {
        if (vis[y][x] == false && dist[y][x] < d) {
          x0 = x;
          y0 = y;
          d = dist[y][x];
        }
      }
    }
    if (d == INT64_MAX) break;
  }

  printf("%ld\n", dist[ny - 1][nx - 1]);
}
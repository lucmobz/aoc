#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1
#define HEIGHT 41
#define WIDTH_NEWLINE 81
#define WIDTH 80
#else
#define HEIGHT 5
#define WIDTH_NEWLINE 9
#define WIDTH 8
#endif

void dump_visited(const char *grid, const bool *visited, int x0, int y0) {
  for (size_t y = 0; y < HEIGHT; ++y) {
    for (size_t x = 0; x < WIDTH; ++x) {
      if ((int)x == x0 && (int)y == y0)
        putchar('$');
      if (visited[y * WIDTH + x])
        putchar('*');
      else
        putchar('.');
    }
    putchar('\n');
  }
  printf("%d, %d, %c\n", x0, y0, grid[y0 * WIDTH_NEWLINE + x0]);
  putchar('\n');
}

// Dijkstra's shortest distance
int main(void) {
  char grid[HEIGHT][WIDTH_NEWLINE] = {0};
  fread(grid, sizeof(char), HEIGHT * WIDTH_NEWLINE, stdin);
  fclose(stdin);

  // Find source and destination coordinates
  int src[2];
  int dst[2];

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (grid[y][x] == 'S') {
        dst[0] = x;
        dst[1] = y;
        grid[y][x] = 'a';
      } else if (grid[y][x] == 'E') {
        src[0] = x;
        src[1] = y;
        grid[y][x] = 'z';
      }
    }
  }

  int distances[HEIGHT][WIDTH] = {0};
  for (size_t i = 0; i < HEIGHT * WIDTH; ++i)
    ((int *)distances)[i] = INT_MAX;
  distances[src[1]][src[0]] = 0;

  bool visited[HEIGHT][WIDTH] = {0};

  int x0 = src[0];
  int y0 = src[1];
  int dx[4] = {1, 0, -1, 0};
  int dy[4] = {0, 1, 0, -1};

  while (x0 != dst[0] || y0 != dst[1]) {
    visited[y0][x0] = true;
    // dump_visited((char *)grid, (bool *)visited, x0, y0);

    for (int i = 0; i < 4; ++i) {
      int x = x0 + dx[i];
      int y = y0 + dy[i];

      if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || visited[y][x] ||
          grid[y0][x0] > 1 + grid[y][x])
        continue;

      int d = distances[y][x];
      int d0 = distances[y0][x0];

      if (d > d0 + 1)
        distances[y][x] = d0 + 1;
    }

    // Find node with current minimal distance
    int d_min = INT_MAX;
    int x_min = -1;
    int y_min = -1;

    for (int y = 0; y < HEIGHT; ++y) {
      for (int x = 0; x < WIDTH; ++x) {
        if (!visited[y][x] && distances[y][x] < d_min) {
          d_min = distances[y][x];
          x_min = x;
          y_min = y;
        }
      }
    }

    if (x_min < 0 || y_min < 0)
      exit(1);

    x0 = x_min;
    y0 = y_min;
  }

  printf("Part 1: %d\n", distances[y0][x0]);

  // Just flip start and end and fix the connection rules
  int d_min = INT_MAX;
  int x_min = -1;
  int y_min = -1;
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (grid[y][x] == 'a' && distances[y][x] < d_min) {
        d_min = distances[y][x];
        x_min = x;
        y_min = y;
      }
    }
  }

  printf("Part 2: %d\n", distances[y_min][x_min]);
}
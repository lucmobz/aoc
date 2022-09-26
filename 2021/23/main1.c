#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Graph layout

 8-9-10-11-12-13-14-15-16-17-18
      |    |      |    |
      1    3      5    7
      |    |      |    |
      0    2      4    6
 */

#define GSZ 19

static int pos[8] = {0};
static int paths[32][32][32];
static int graph[32][4];

void init_graph() {
  graph[0][0] = 1;
  graph[0][1] = -1;

  graph[1][0] = 0;
  graph[1][1] = 10;
  graph[1][2] = -1;

  graph[2][0] = 3;
  graph[2][1] = -1;

  graph[3][0] = 2;
  graph[3][1] = 12;
  graph[3][2] = -1;

  graph[4][0] = 5;
  graph[4][1] = -1;

  graph[5][0] = 4;
  graph[5][1] = 14;
  graph[5][2] = -1;

  graph[6][0] = 7;
  graph[6][1] = -1;

  graph[7][0] = 6;
  graph[7][1] = 16;
  graph[7][2] = -1;

  graph[8][0] = 9;
  graph[8][1] = -1;

  graph[9][0] = 8;
  graph[9][1] = 10;
  graph[9][2] = -1;

  graph[10][0] = 1;
  graph[10][1] = 9;
  graph[10][2] = 11;
  graph[10][3] = -1;

  graph[11][0] = 10;
  graph[11][1] = 12;
  graph[11][2] = -1;

  graph[12][0] = 3;
  graph[12][1] = 11;
  graph[12][2] = 13;
  graph[12][3] = -1;

  graph[13][0] = 12;
  graph[13][1] = 14;
  graph[13][2] = -1;

  graph[14][0] = 5;
  graph[14][1] = 13;
  graph[14][2] = 15;
  graph[14][3] = -1;

  graph[15][0] = 14;
  graph[15][1] = 16;
  graph[15][2] = -1;

  graph[16][0] = 7;
  graph[16][1] = 15;
  graph[16][2] = 17;
  graph[16][3] = -1;

  graph[17][0] = 16;
  graph[17][1] = 18;
  graph[17][2] = -1;

  graph[18][0] = 17;
  graph[18][1] = -1;
}

void find_path(bool* reached, int p1, int p2, int* path, int sz,
               bool* visited) {
  if (p1 == p2) {
    if (sz > 0) path[sz - 1] = -1;
    *reached = true;
    return;
  }

  bool found = false;
  for (int i = 0; graph[p1][i] != -1; ++i) {
    int p = graph[p1][i];
    if (visited[p]) continue;

    visited[p] = true;
    path[sz] = p;
    found = true;
    find_path(reached, p, p2, path, sz + 1, visited);
    if (*reached) return;

    found = false;
    path[sz] = -1;
    visited[p] = false;
  }
  if (!found) return;
}

void init_paths() {
  for (int p1 = 0; p1 < GSZ; ++p1) {
    for (int p2 = 0; p2 < GSZ; ++p2) {
      bool visited[GSZ] = {0};
      visited[p1] = true;
      memset(paths[p1][p2], -1, sizeof(int) * 32);
      bool reached = false;
      find_path(&reached, p1, p2, paths[p1][p2], 0, visited);
    }
  }
}

void dump_path(int p1, int p2) {
  printf("[%d]", p1);
  for (int i = 0; paths[p1][p2][i] != -1; ++i) {
    putchar(' ');
    printf("%d", (int)paths[p1][p2][i]);
  }
  printf(" [%d]\n", p2);
}

void read_init_pos() {
  char line[64];

  fgets(line, 64, stdin);
  fgets(line, 64, stdin);
  fgets(line, 64, stdin);

  bool found[4] = {0};

  for (int i = 1; i < 8; i += 2) {
    int c = line[2 + i] - 'A';
    if (!found[c]) {
      found[c] = true;
      pos[2 * c] = i;
    } else
      pos[2 * c + 1] = i;
  }

  fgets(line, 64, stdin);

  for (int i = 0; i < 8; i += 2) {
    int c = line[3 + i] - 'A';
    if (!found[c]) {
      found[c] = true;
      pos[2 * c] = i;
    } else
      pos[2 * c + 1] = i;
  }
}

static inline int pos2ch(int p) {
  static int ch[] = {31, 17, 33, 19, 35, 21, 37, 23};
  if (p >= 8) return p - 7;
  return ch[p];
}

void dump_pos() {
  puts("#############");
  char str[] = "#...........#\n###.#.#.#.###\n  #.#.#.#.#  ";
  str[pos2ch(pos[0])] = 'A';
  str[pos2ch(pos[1])] = 'A';
  str[pos2ch(pos[2])] = 'B';
  str[pos2ch(pos[3])] = 'B';
  str[pos2ch(pos[4])] = 'C';
  str[pos2ch(pos[5])] = 'C';
  str[pos2ch(pos[6])] = 'D';
  str[pos2ch(pos[7])] = 'D';
  puts(str);
  puts("  #########  ");
}

static inline bool is_path_free(int p1, int p2, uint64_t* steps) {
  for (int i = 0; i < 8; ++i)
    if (pos[i] == p2) return false;

  *steps = 0;
  for (int j = 0; paths[p1][p2][j] != -1; ++j) {
    int p = paths[p1][p2][j];
    for (int i = 0; i < 8; ++i)
      if (pos[i] == p) return false;
    ++(*steps);
  }
  ++(*steps);

  return true;
}

static uint64_t costs[] = {1, 10, 100, 1000};

bool reachable(int p1, int p2, int i, uint64_t* cost) {
  *cost = 0;
  if (p1 == p2) return false;
  uint64_t steps = 0;
  if (is_path_free(p1, p2, &steps)) {
    *cost = steps * costs[i / 2];
    return true;
  }
  return false;
}

static inline bool ok(int i) { return (i / 2) == ((int)pos[i] / 2); }

void simulate(uint64_t energy, uint64_t* lowest, uint64_t depth) {
  //printf("%lu\n", depth);
  //dump_pos();
  bool done = true;

  for (int i = 0; i < 8; ++i) {
    if (ok(i) && ok(i ^ 1)) continue;

    if (pos[i] >= 8) {
      done = false;

      int p = (i / 2) * 2;
      uint64_t cost = 0;
      if (reachable(pos[i], p, i, &cost)) {
        int p0 = pos[i];
        pos[i] = p;
        simulate(energy + cost, lowest, depth + 1);
        pos[i] = p0;
      } else if (ok(i ^ 1) && pos[i ^ 1] % 2 == 0 &&
                 reachable(pos[i], p + 1, i, &cost)) {
        int p0 = pos[i];
        pos[i] = p;
        simulate(energy + cost, lowest, depth + 1);
        pos[i] = p0;
      }

    } else if (!ok(i) || (ok(i) && !ok(i ^ 1) && pos[i] % 2 == 1)) {
      done = false;

      for (int p = 8; p < GSZ; ++p) {
        if (p == 10 || p == 12 || p == 14 || p == 16) continue;

        uint64_t cost = 0;
        if (reachable(pos[i], p, i, &cost)) {
          int p0 = pos[i];
          pos[i] = p;
          simulate(energy + cost, lowest, depth + 1);
          pos[i] = p0;
        }
      }
    }
  }

  if (done && energy < *lowest) *lowest = energy;
}

int main(void) {
  init_graph();
  read_init_pos();
  init_paths();

  //for (int p1 = 0; p1 < 15; ++p1)
  //  for (int p2 = 0; p2 < 15; ++p2) dump_path(p1, p2);

  uint64_t lowest = UINT64_MAX;
  simulate(0, &lowest, 0);
  printf("Part 1: %lu\n", lowest);
}
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H 11
#define T 4
#define R 4

static char hallway[H] = {0};
static char rooms[T][R] = {0};

void read_map() {
  char line[64];

  fgets(line, 64, stdin);
  fgets(line, 64, stdin);
  fgets(line, 64, stdin);
  for (int i = 0; i < T; ++i) {
    rooms[i][0] = line[3 + 2 * i];
  }

  rooms[0][1] = 'D';
  rooms[1][1] = 'C';
  rooms[2][1] = 'B';
  rooms[3][1] = 'A';
  rooms[0][2] = 'D';
  rooms[1][2] = 'B';
  rooms[2][2] = 'A';
  rooms[3][2] = 'C';
  
  fgets(line, 64, stdin);
  for (int i = 0; i < T; ++i) {
    rooms[i][R - 1] = line[3 + 2 * i];
  }
}

void dump_map(int depth) {
  printf("%d\n", depth);
  printf("%*s", depth, "");
  puts("#############");
  printf("%*s", depth, "");
  putchar('#');
  for (int i = 0; i < H; ++i) {
    if (hallway[i] != 0)
      putchar(hallway[i]);
    else
      putchar('.');
  }
  putchar('#');
  putchar('\n');
  printf("%*s", depth, "");
  printf("###");
  for (int i = 0; i < 4; ++i) {
    char c = rooms[i][0];
    printf("%c#", c ? c : '.');
  }
  printf("##\n");
  for (int i = 1; i < R; ++i) {
    printf("%*s", depth, "");
    printf("  #");
    for (int j = 0; j < T; ++j) {
      char c = rooms[j][i];
      printf("%c#", c ? c : '.');
    }
    putchar('\n');
  }
  printf("%*s", depth, "");
  puts("  #########");
}

static uint64_t costs[] = {1, 10, 100, 1000};
static int doors[] = {2, 4, 6, 8};

int find_path(int p1, int p2) {
  if (p1 == p2) return -1;
  int p;
  int dp = p1 < p2 ? 1 : -1;
  for (p = p1; p != p2; p += dp)
    if (hallway[p + dp]) return -1;
  return (p - p1) * dp;
}

int find_room(int r) {
  int good = 0;
  int bad = 0;
  for (int i = 0; i < R; ++i) {
    if (rooms[r][i] == r + 'A')
      ++good;
    else if (rooms[r][i])
      ++bad;
  }

  if (bad)
    return -1;
  else
    return R - 1 - good;
}

int room_good(int r) {
  int good = 0;
  for (int i = R - 1; i >= 0; --i) {
    if (rooms[r][i] == r + 'A') ++good;
  }
  return good;
}

void simulate(uint64_t energy, uint64_t* lowest, uint64_t depth) {
  //dump_map(depth);

  for (int i = 0; i < H; ++i) {
    char c = hallway[i];
    if (c) {
      int r = c - 'A';

      int pos;
      if ((pos = find_room(r)) != -1) {
        int steps;
        if ((steps = find_path(i, doors[r])) != -1) {
          uint64_t cost = (steps + pos + 1) * costs[r];
          char old = rooms[r][pos];

          hallway[i] = 0;
          rooms[r][pos] = c;
          simulate(energy + cost, lowest, depth + 1);
          hallway[i] = c;
          rooms[r][pos] = old;
        }
      }
    }
  }

  bool all_good = true;

  for (int r = 0; r < T; ++r) {
    int good;
    if ((good = room_good(r)) < R) {
      all_good = false;

      for (int pos = 0; pos < R - good; ++pos) {
        char c = rooms[r][pos];
        int i = c - 'A';

        if (c) {
          int skip = false;
          int p = pos - 1;
          while (p >= 0) {
            if (rooms[r][p]) skip = true;
            --p;
          }

          if (!skip) {
            if (!hallway[doors[r] + 1] || !hallway[doors[r] - 1]) {
              for (int p2 = 0; p2 < H; ++p2) {
                if (p2 == doors[0] || p2 == doors[1] || p2 == doors[2] ||
                    p2 == doors[3])
                  continue;

                int steps;
                if ((steps = find_path(doors[r], p2)) != -1) {
                  uint64_t cost = (steps + pos + 1) * costs[i];

                  hallway[p2] = c;
                  rooms[r][pos] = 0;
                  simulate(energy + cost, lowest, depth + 1);
                  hallway[p2] = 0;
                  rooms[r][pos] = c;
                }
              }
            }
          }
        }
      }
    }
  }

  if (all_good && energy < *lowest) {
    for (int r = 0; r < T; ++r) {
      if (room_good(r) != R) {
        puts("ERROR");
        exit(EXIT_FAILURE);
      }
    }
    *lowest = energy;
  }
}

int main(void) {
  read_map();

  uint64_t lowest = UINT64_MAX;
  simulate(0, &lowest, 0);
  printf("Part 2: %lu\n", lowest);
}
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASZ 1024
#define ISZ 1024

char algo[ASZ] = {0};
size_t asz = 0;

char img[ISZ][ISZ];
int nx = 0;
int ny = 0;

void dump_img() {
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      putchar(img[y][x]);
    }
    putchar('\n');
  }
  putchar('\n');
}

void clear_img() {
  for (int y = 0; y < ISZ; ++y) {
    for (int x = 0; x < ISZ; ++x) {
      img[y][x] = 0;
    }
  }
}

void expand_img(char c) {
  char tmp[ISZ][ISZ];

  ny += 2;
  nx += 2;

  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      if (y <= 0 || x <= 0 || y >= ny - 1 || x >= nx - 1)
        tmp[y][x] = c;
      else
        tmp[y][x] = img[y - 1][x - 1];
    }
  }

  clear_img();
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      img[y][x] = tmp[y][x];
    }
  }
}

size_t get_index(int x0, int y0, char c) {
  char bits[10] = {0};
  size_t bsz = 0;

  for (int y = y0 - 1; y <= y0 + 1; ++y) {
    for (int x = x0 - 1; x <= x0 + 1; ++x) {
      if (y < 0 || y >= ny || x < 0 || x >= nx) {
        bits[bsz++] = c == '#' ? '1' : '0';
      } else {
        bits[bsz++] = (img[y][x] == '#' ? '1' : '0');
        assert(img[y][x] == '#' || img[y][x] == '.');
      }
    }
  }

  assert(bsz <= 9);
  return strtol(bits, NULL, 2);
}

void update_img(char c) {
  char tmp[ISZ][ISZ] = {0};

  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      size_t i = get_index(x, y, c);
      assert(i < asz);
      tmp[y][x] = algo[i];
    }
  }

  clear_img();
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      img[y][x] = tmp[y][x];
    }
  }
}

uint64_t count_lit() {
  uint64_t count = 0;
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      if (img[y][x] == '#') ++count;
    }
  }
  return count;
}

int main(void) {
  fgets(algo, 1024, stdin);
  algo[strlen(algo) - 1] = 0;
  asz = strlen(algo);

  char line[4096];
  fgets(line, 4096, stdin);
  while (fgets(line, 4096, stdin)) {
    if (nx == 0) nx = strlen(line) - 1;
    for (int x = 0; x < nx; ++x) img[ny][x] = line[x];
    ++ny;
  }

  for (size_t i = 0; i < 50; ++i) {
    // Set up the char at infinity
    char c = i % 2 ? '#' : '.';
    expand_img(c);
    update_img(c);
    if (i == 1) printf("Part 1: %lu\n", count_lit());
  }
  printf("Part 2: %lu\n", count_lit());
}
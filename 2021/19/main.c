#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SS_SZ 64

typedef struct Point {
  int64_t x, y, z;
} Point;

typedef struct Scanner {
  size_t sz;
  Point pts[64];
} Scanner;

typedef struct Map {
  size_t sz;
  Point pts[SS_SZ * 64];
  bool id[SS_SZ * 64];
} Map;

void dump_point(Point p) { printf("%6ld %6ld %6ld\n", p.x, p.y, p.z); }

void dump_scanner(Scanner* s) {
  printf("     X      Y      Z (%zu)\n", s->sz);
  for (size_t i = 0; i < 64 && i < s->sz; ++i) dump_point(s->pts[i]);
  putchar('\n');
}

bool read_scanner(Scanner* s, FILE* file) {
  int id;

  if (fscanf(file, "--- scanner %d ---\n", &id) != 1) return false;

  char line[64];

  while (fgets(line, 64, file)) {
    if (line[0] == '\n') break;

    sscanf(line, "%ld,%ld,%ld", &(s->pts[s->sz].x), &(s->pts[s->sz].y),
           &(s->pts[s->sz].z));
    ++s->sz;
  }

  if (feof(file))
    return false;
  else
    return true;
}

bool match(Scanner* s1, Scanner* s2) {
  int count = 0;

  for (size_t i = 0; i < s1->sz; ++i) {
    for (size_t j = 0; j < s2->sz; ++j) {
      Point p1 = s1->pts[i];
      Point p2 = s2->pts[j];

      if (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z) ++count;
      if (count >= 12) return true;
    }
  }

  return false;
}

void shift(Scanner* s, Point p) {
  for (size_t i = 0; i < s->sz; ++i) {
    s->pts[i].x += p.x;
    s->pts[i].y += p.y;
    s->pts[i].z += p.z;
  }
}

void rot_x(Scanner* s) {
  for (size_t i = 0; i < s->sz; ++i) {
    int64_t tmp = s->pts[i].y;
    s->pts[i].y = -s->pts[i].z;
    s->pts[i].z = tmp;
  }
}

void rot_y(Scanner* s) {
  for (size_t i = 0; i < s->sz; ++i) {
    int64_t tmp = s->pts[i].z;
    s->pts[i].z = -s->pts[i].x;
    s->pts[i].x = tmp;
  }
}

void rot_z(Scanner* s) {
  for (size_t i = 0; i < s->sz; ++i) {
    int64_t tmp = s->pts[i].x;
    s->pts[i].x = -s->pts[i].y;
    s->pts[i].y = tmp;
  }
}

void orient(Scanner* s, size_t id) {
  if (id <= 3) {
    for (size_t i = 0; i != id; ++i) rot_z(s);
  } else if (4 <= id && id < 8) {
    rot_x(s);
    rot_x(s);
    for (size_t i = 4; i != id; ++i) rot_z(s);
  } else if (8 <= id && id < 12) {
    rot_x(s);
    for (size_t i = 8; i != id; ++i) rot_z(s);
  } else if (12 <= id && id < 16) {
    rot_x(s);
    rot_x(s);
    rot_x(s);
    for (size_t i = 12; i != id; ++i) rot_z(s);
  } else if (16 <= id && id < 20) {
    rot_y(s);
    for (size_t i = 16; i != id; ++i) rot_z(s);
  } else if (20 <= id && id < 24) {
    rot_y(s);
    rot_y(s);
    rot_y(s);
    for (size_t i = 20; i != id; ++i) rot_z(s);
  } else {
    assert(false);
  }
}

int count_beacons(Scanner* ss, size_t ss_sz) {
  Point beacons[SS_SZ * 64];
  size_t sz = 0;

  for (size_t i = 0; i < ss[0].sz; ++i) beacons[sz++] = ss[0].pts[i];

  for (size_t i = 1; i < ss_sz; ++i) {
    for (size_t j = 0; j < ss[i].sz; ++j) {
      Point p = ss[i].pts[j];
      bool found = false;

      for (size_t k = 0; k < sz; ++k) {
        Point b = beacons[k];

        if (p.x == b.x && p.y == b.y && p.z == b.z) {
          found = true;
          break;
        }
      }

      if (!found) beacons[sz++] = p;
    }
  }

  return sz;
}

int64_t max_manhattan(Point* spos, size_t ss_sz) {
  int64_t max = 0;

  for (size_t i = 0; i < ss_sz; ++i) {
    for (size_t j = 0; j < ss_sz; ++j) {
      if (i == j) continue;

      int64_t dx = spos[i].x - spos[j].x;
      int64_t dy = spos[i].y - spos[j].y;
      int64_t dz = spos[i].z - spos[j].z;
      dx *= dx < 0 ? -1 : 1;
      dy *= dy < 0 ? -1 : 1;
      dz *= dz < 0 ? -1 : 1;
      int64_t d = dx + dy + dz;

      if (max < d) max = d;
    }
  }
  
  return max;
}

int main(void) {
  Scanner ss[SS_SZ];
  size_t ss_sz = 0;
  Point spos[SS_SZ];

  while (read_scanner(ss + ss_sz, stdin)) {
    // dump_scanner(&ss[ss_sz]);
    ++ss_sz;
  }
  ++ss_sz;
  // dump_scanner(&ss[ss_sz]);

  bool visited[SS_SZ] = {0};

  size_t q[SS_SZ];
  size_t qsz = 0;
  size_t qft = 0;

  q[qsz++] = 0;
  visited[0] = true;
  spos[0].x = 0;
  spos[0].y = 0;
  spos[0].z = 0;

  while (qft < qsz) {
    size_t i = q[qft++];
    Scanner* s1 = ss + i;

    for (size_t j = 0; j < ss_sz; ++j) {
      if (visited[j]) continue;
      Scanner* s2 = &ss[j];
      //bool matched = false;

      for (size_t k = 0; k < 24; ++k) {
        for (size_t a = 0; a < s1->sz; ++a) {
          for (size_t b = 0; b < s2->sz; ++b) {
            Scanner s = *s2;
            orient(&s, k);

            Point p1 = s1->pts[a];
            Point p2 = s.pts[b];
            Point p = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
            shift(&s, p);

            if (match(s1, &s)) {
              q[qsz++] = j;
              visited[j] = true;
              spos[j] = p;
              *s2 = s;
              //matched = true;
              goto end;
            }
          }
        }
      }

    end:
      //if (matched) printf("Match %zu %zu\n", i, j);
    }
  }

  printf("Part 1: %d\n", count_beacons(ss, ss_sz));
  printf("Part 2: %ld\n", max_manhattan(spos, ss_sz));
}

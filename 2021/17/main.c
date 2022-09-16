#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

typedef int64_t i64;
static i64 xmin, xmax, ymin, ymax;

void update(i64* x, i64* y, i64* u, i64* v) {
  *x += *u;
  *y += *v;
  if (*u > 0)
    *u -= 1;
  else if (*u < 0)
    *u += 1;
  *v -= 1;
}

bool hit(i64 x, i64 y) {
  if (xmin <= x && x <= xmax && ymin <= y && y <= ymax) return true;
  return false;
}

bool stop(i64 x, i64 y, i64 u, i64 v) {
  if (x > xmax && u >= 0) return true;
  if (x < xmin && u <= 0) return true;
  if (y < ymin && v <= 0) return true;

  return false;
}

i64 compute_umin(i64 xmin) {
  for (i64 u0 = 0;; ++u0) {
    i64 x = 0;
    i64 u = u0;
    while (u > 0 && x < xmin) {
      x += u;
      u -= 1;
    }
    if (x >= xmin) return u0;
  }
  return -1;
}

int main(void) {
  i64 x0 = 0;
  i64 y0 = 0;
  i64 u0 = 0;
  i64 v0 = 0;

  scanf("target area: x=%ld..%ld, y=%ld..%ld", &xmin, &xmax, &ymin, &ymax);
  printf("x = [%ld, %ld]\ny = [%ld, %ld]\n", xmin, xmax, ymin, ymax);

  i64 umin = compute_umin(xmin);
  i64 h = 0;
  i64 uh = 0;
  i64 vh = 0;
  i64 hits = 0;

  for (u0 = umin; u0 <= xmax; ++u0) {
    for (v0 = ymin; v0 <= labs(ymin); ++v0) {
      i64 x = x0;
      i64 y = y0;
      i64 u = u0;
      i64 v = v0;
      i64 yh = h;

      while (!stop(x, y, u, v) && !hit(x, y)) {
        update(&x, &y, &u, &v);
        if (y > yh) yh = y;
      }

      if (hit(x, y) && yh > h){
        uh = u0;
        vh = v0;
        h = yh;
      }

      if (hit(x, y)) ++hits;
    }
  }

  printf("highest (u0, v0) = (%ld, %ld)\nhighest y = %ld\nhits = %ld\n", uh, vh, h, hits);

}
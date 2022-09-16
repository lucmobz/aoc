#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void reserve(int64_t** xs, size_t* cap, size_t sz) {
  if (sz <= *cap) return;

  if (*cap == 0) *cap = 1;
  while (*cap <= sz) *cap *= 2;

  *xs = realloc(*xs, sizeof(int64_t) * *cap);
}

int64_t less(const void* lhs, const void* rhs) {
  if (*(int64_t*)lhs < *(int64_t*)rhs)
    return -1;
  else if (*(int64_t*)lhs > *(int64_t*)rhs)
    return 1;
  else
    return 0;
}

int64_t cost_func(int64_t* xs, size_t sz, int64_t best_x, int64_t best_cost) {
  int64_t cost = 0;

  for (size_t i = 0; i < sz; ++i) {
    if (cost > best_cost * 2) return INT_MAX;
    int dist = labs(xs[i] - best_x);
    cost += dist * (dist + 1);
  }

  return cost / 2;
}

int main(void) {
  size_t sz = 0;
  size_t cap = 1;
  int64_t* xs = malloc(sizeof(int64_t) * cap);

  if (xs == NULL) {
    fprintf(stderr, "ERROR: %s\n", strerror(errno));
    return 1;
  }

  int64_t x, xmax = INT_MIN, xmin = INT_MAX;

  while (scanf("%ld%*c", &x) == 1) {
    reserve(&xs, &cap, sz);
    xs[sz++] = x;
    if (x > xmax) xmax = x;
    if (x < xmin) xmin = x;
  }

  xs = realloc(xs, sizeof(int64_t) * cap);

  /* Part 1: minimizer of the 1-norm is the median
  qsort(xs, sz, sizeof(int64_t), less);

  int64_t m = xs[sz / 2];
  printf("%d\n", m);

  int64_t fuel = 0;
  for (size_t i = 0; i < sz; ++i) fuel += labs(xs[i] - m);
  printf("%d\n", fuel);
  */

  int64_t best_cost = cost_func(xs, sz, xmin, INT_MAX);
  int64_t best_x = xmin;

  for (int64_t x = xmin + 1; x <= xmax; ++x) {
    int64_t new_cost = cost_func(xs, sz, x, best_cost);
    if (new_cost < best_cost) {
      best_cost = new_cost;
      best_x = x;
    }
  }

  printf("Position: %ld\nCost: %ld\n", best_x, best_cost);

  free(xs);
}

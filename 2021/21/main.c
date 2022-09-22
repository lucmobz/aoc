#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SCORE 21

static uint64_t count = 0;

uint64_t roll() { return count++ % 100 + 1; }

static uint64_t freq[] = {1, 3, 6, 7, 6, 3, 1};

void simulate(uint64_t n, uint64_t p1, uint64_t p2, uint64_t s1, uint64_t s2, uint64_t* w1,
              uint64_t* w2) {
  assert(s1 < 21 && s2 < 21);

  for (uint64_t dp1 = 3; dp1 <= 9; ++dp1) {
    uint64_t new_p1 = (p1 + dp1 - 1) % 10 + 1;
    uint64_t new_s1 = s1 + new_p1;
    uint64_t n1 = freq[dp1 - 3] * n;

    if (new_s1 >= MAX_SCORE) {
      *w1 += n1;
    } else {
      for (uint64_t dp2 = 3; dp2 <= 9; ++dp2) {
        uint64_t new_p2 = (p2 + dp2 - 1) % 10 + 1;
        uint64_t new_s2 = s2 + new_p2;
        uint64_t n2 = freq[dp2 - 3] * n1;

        if (new_s2 >= MAX_SCORE) {
          *w2 += n2;
        } else {
          simulate(n2, new_p1, new_p2, new_s1,
                   new_s2, w1, w2);
        }
      }
    }
  }
}

int main(void) {
  uint64_t p1;
  uint64_t p2;
  uint64_t s1 = 0;
  uint64_t s2 = 0;

  if (2 != scanf("Player 1 starting position: %lu\nPlayer 2 starting "
                 "position: %lu",
                 &p1, &p2))
    exit(EXIT_FAILURE);

  // Part 1
  uint64_t new_p1 = p1;
  uint64_t new_p2 = p2;
  while (true) {
    new_p1 = (new_p1 + roll() + roll() + roll() - 1) % 10 + 1;
    s1 += new_p1;
    if (s1 >= 1000) break;

    new_p2 = (new_p2 + roll() + roll() + roll() - 1) % 10 + 1;
    s2 += new_p2;
    if (s2 >= 1000) break;
  }

  printf("Part 1: ");
  if (s1 >= 1000)
    printf("%lu\n", s2 * count);
  else if (s2 >= 1000)
    printf("%lu\n", s1 * count);

  // Part 2
  uint64_t w2 = 0;
  uint64_t w1 = 0;
  simulate(1, p1, p2, 0, 0, &w1, &w2);
  printf("Part 2: %lu\n", w1 > w2 ? w1 : w2);
}
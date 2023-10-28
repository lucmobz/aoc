#include <stdint.h>
#include <stdio.h>

#define LINE_SIZE 8

static char line[LINE_SIZE] = {0};

// player score, row: opponent, col: player
static uint64_t score_round[3][3] = {{3, 6, 0}, {0, 3, 6}, {6, 0, 3}};
static uint64_t score_shape[3] = {1, 2, 3};

int main(void) {
  uint64_t score = 0;
  while (fgets(line, LINE_SIZE, stdin)) {
    size_t opponent = line[0] - 'A';
    size_t player = line[2] - 'X';

    score += score_round[opponent][player] + score_shape[player];
  }

  printf("Part 1: %lu\n", score);
}

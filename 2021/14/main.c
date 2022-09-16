#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char rules[26][26] = {0};
static uint64_t table[26][26] = {0};

void dump_rules() {
  printf("  ");
  for (size_t i = 0; i < 26; ++i) putchar((char)('A' + i));
  putchar('\n');
  for (size_t i = 0; i < 26; ++i) {
    printf("%c ", (char)('A' + i));
    for (size_t j = 0; j < 26; ++j) {
      if (rules[i][j])
        putchar((char)(rules[i][j] + 'A'));
      else
        putchar('.');
    }
    putchar('\n');
  }
}

void dump_table() {
  printf("  ");
  for (size_t i = 0; i < 26; ++i) printf("%c ", 'A' + (char)i);
  putchar('\n');
  for (size_t i = 0; i < 26; ++i) {
    printf("%c ", 'A' + (char)i);
    for (size_t j = 0; j < 26; ++j) {
      printf("%lu ", table[i][j]);
    }
    putchar('\n');
  }
}

void update_table() {
  static uint64_t back_table[26][26];
  memset(back_table, 0, sizeof(back_table));

  for (size_t i = 0; i < 26; ++i) {
    for (size_t j = 0; j < 26; ++j) {
      back_table[i][(size_t)rules[i][j]] += table[i][j];
      back_table[(size_t)rules[i][j]][j] += table[i][j];
    }
  }

  memcpy(table, back_table, sizeof(table));
}

int main(void) {
  char template[4096];

  fgets(template, 4096, stdin);
  getchar();

  char x, y, z;
  while (scanf("%c%c -> %c\n", &x, &y, &z) == 3) {
    rules[x - 'A'][y - 'A'] = z - 'A';
  }

  for (size_t i = 0; i < strlen(template) - 2; ++i) {
    size_t x = template[i] - 'A';
    size_t y = template[i + 1] - 'A';
    ++table[x][y];
  }

  for (size_t i = 0; i < 40; ++i) {
    update_table();
  }

  uint64_t freq[26] = {0};
  for (size_t i = 0; i < 26; ++i) {
    for (size_t j = 0; j < 26; ++j) {
      freq[i] += table[i][j];
    }
  }
  freq[template[strlen(template) - 2] - 'A'] += 1;

  uint64_t min = UINT64_MAX;
  uint64_t max = 0;
  for (size_t i = 0; i < 26; ++i) {
    uint64_t n = freq[i];
    if (n > 0) {
      min = n < min ? n : min;
      max = n > max ? n : max;
    }
  }

  printf("%lu\n", max - min);
}
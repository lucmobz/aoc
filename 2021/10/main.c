#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

bool is_open(char c) {
  if (c == '(' || c == '[' || c == '{' || c == '<') return true;
  return false;
}

bool is_close(char c) {
  if (c == ')' || c == ']' || c == '}' || c == '>') return true;
  return false;
}

int64_t get_score(char c) {
  if (c == ')') return 3;
  if (c == ']') return 57;
  if (c == '}') return 1197;
  if (c == '>') return 25137;

  assert(false);
  return 0;
}

int64_t get_score2(char c) {
  if (c == ')') return 1;
  if (c == ']') return 2;
  if (c == '}') return 3;
  if (c == '>') return 4;

  assert(false);
  return 0;
}

char get_match(char c) {
  if (c == '(') return ')';
  if (c == ')') return '(';
  if (c == '[') return ']';
  if (c == ']') return '[';
  if (c == '{') return '}';
  if (c == '}') return '{';
  if (c == '<') return '>';
  if (c == '>') return '<';

  assert(false);
  return 0;
}

int less(const void* l, const void* r) {
  int64_t ll = *(int64_t*)l;
  int64_t rr = *(int64_t*)r;

  /*
  if (ll - rr > INT32_MAX || ll - rr < INT32_MIN) {
    fprintf(stderr, "ERROR: int64_t overflow\n");
    exit(1);
  }
  */
  
  if (ll < rr)
    return -1;
  else if (ll > rr)
    return 1;
  else
    return 0;
}

int main(void) {
  int64_t score = 0;
  int64_t scores[1 << 16] = {0};
  size_t scores_sz = 0;

  char line[4096] = {0};
  size_t line_count = 0;

  while (fgets(line, 4096, stdin)) {
    bool corrupt = false;

    char stack[1 << 16] = {0};
    size_t sz = 0;

    ++line_count;
    size_t len = strlen(line);
    if (line[len - 1] == '\n') line[--len] = '\0';

    for (size_t i = 0; i < len; ++i) {
      char c = line[i];

      if (is_open(c)) {
        stack[sz++] = c;
      } else if (is_close(c)) {
        if (sz == 0 || c != get_match(stack[sz - 1])) {
          score += get_score(c);
          corrupt = true;
          break;
        } else {
          assert(sz > 0);
          --sz;
        }
      } else {
        fprintf(stderr, "Unreachable\n");
        return 1;
      }
    }

    if (corrupt) continue;

    printf("Line: %s", line);

    char seq[4096] = {0};
    size_t seq_sz = 0;

    while (sz > 0) {
      char c = get_match(stack[--sz]);
      seq[seq_sz++] = c;
    }

    int64_t score2 = 0;
    printf(" : %llu : ", line_count);

    for (size_t i = 0; i < seq_sz; ++i) {
      char c = seq[i];
      putchar(c);
      score2 *= 5;
      score2 += get_score2(c);
    }
    printf(" : %llu\n\n", score2);

    scores[scores_sz++] = score2;
  }

  printf("%llu\n", score);
  qsort(scores, scores_sz, sizeof(int64_t), less);
  printf("%llu : %llu\n", scores_sz, scores[scores_sz / 2]);
}
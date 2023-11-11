#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_SIZE 256
#define PACKET_COUNT 512

void print_token(const char *p, const char *end) {
  while (p && end && p != end)
    putchar(*p++);
  putchar('\n');
}

char *get_token(char *p, char *end) {
  assert(p && end);
  assert(p <= end);
  assert(p == end || *p == '[' || isdigit(*p));
  assert(*end == ']' || *end == ',');

  // No more tokens
  if (p == end)
    return NULL;

  if (isdigit(*p)) {
    char *e = p;
    strtol(e, &e, 10);
    assert(*e == ',' || *e == ']');
    return e;
  }

  if (*p == '[') {
    char *e = p + 1;
    int paren = 1;
    while (e < end && paren > 0) {
      if (*e == '[')
        ++paren;
      else if (*e == ']')
        --paren;
      ++e;
    }
    return e;
  }

  assert(false);
  return NULL;
}

int compare_packets(char *p1, char *end1, char *p2, char *end2) {
  char *e1 = get_token(p1, end1);
  char *e2 = get_token(p2, end2);

  // print_token(p1, e1);
  // print_token(p2, e2);

  while (e1 && e2) {
    if (isdigit(*p1) && isdigit(*p2)) {
      int x1 = strtol(p1, NULL, 10);
      int x2 = strtol(p2, NULL, 10);
      if (x1 != x2)
        return x1 - x2;
    }

    if (*p1 == '[' || *p2 == '[') {
      int status =
          compare_packets(p1 + 1 * (*p1 == '['), e1 - 1 * (*p1 == '['),
                          p2 + 1 * (*p2 == '['), e2 - 1 * (*p2 == '['));
      if (status != 0)
        return status;
    }

    p1 = e1;
    p2 = e2;
    if (p1 < end1 && *p1 == ',')
      ++p1;
    if (p2 < end2 && *p2 == ',')
      ++p2;
    e1 = get_token(p1, end1);
    e2 = get_token(p2, end2);
    // if (e1)
    //   print_token(p1, e1);
    // if (e2)
    //   print_token(p2, e2);
  }

  if (!e1 && !e2)
    return 0;
  else if (!e1 && e2)
    return -1;
  else if (e1 && !e2)
    return 1;
  else {
    assert(false);
    return 0;
  }
}

int compare_packets_qsort(const void *lhs, const void *rhs) {
  char *p1 = (char *)lhs;
  char *p2 = (char *)rhs;

  // Compare packets
  char *end1 = p1 + strlen(p1);
  while (end1 > p1 && *end1 != ']')
    --end1;

  char *end2 = p2 + strlen(p2);
  while (end2 > p2 && *end2 != ']')
    --end2;

  return compare_packets(p1 + 1, end1, p2 + 1, end2);
}

int main(void) {
  char p1[PACKET_SIZE] = {0};
  char p2[PACKET_SIZE] = {0};
  int pair_index = 1;
  int pair_index_sum = 0;

  char packets[PACKET_COUNT][PACKET_SIZE] = {0};
  size_t packet_count = 0;

  while (fgets(p1, PACKET_SIZE, stdin)) {
    fgets(p2, PACKET_SIZE, stdin);

    // Compare packets
    char *end1 = p1 + strlen(p1);
    while (end1 > p1 && *end1 != ']')
      --end1;

    char *end2 = p2 + strlen(p2);
    while (end2 > p2 && *end2 != ']')
      --end2;

    // Store packets
    strncpy(packets[packet_count++], p1, PACKET_SIZE);
    strncpy(packets[packet_count++], p2, PACKET_SIZE);

    // Strip parens
    bool right_order = compare_packets(p1 + 1, end1, p2 + 1, end2) < 0;

    if (right_order)
      pair_index_sum += pair_index;

    // printf("%d : %d\n", pair_index, right_order);

    // Skip newline and go to next packet pair
    fgets(p1, PACKET_SIZE, stdin);
    ++pair_index;
  }

  printf("Part 1: %d\n", pair_index_sum);

  strncpy(packets[packet_count++], "[[6]]", 6 * sizeof(char));
  strncpy(packets[packet_count++], "[[2]]", 6 * sizeof(char));

  qsort(packets, packet_count, PACKET_SIZE * sizeof(char),
        compare_packets_qsort);

  size_t decoder_key = 1;
  for (size_t i = 0; i < packet_count; ++i) {
    if (!strncmp("[[6]]", packets[i], 6))
      decoder_key *= i + 1;
    if (!strncmp("[[2]]", packets[i], 6))
      decoder_key *= i + 1;
  }

  printf("Part 2: %zu\n", decoder_key);
}
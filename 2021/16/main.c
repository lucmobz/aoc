#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

static char* bits = NULL;
static size_t sz = 0;
static size_t cap = 4096;
static const uint64_t lengths[2] = {15, 11};

void append_bits(char* dst, char c) {
  char x = -1;
  if ('0' <= c && c <= '9')
    x = c - '0';
  else if ('A' <= c && c <= 'F')
    x = 10 + c - 'A';
  else
    assert(false);

  dst += 3;
  do *dst-- = x % 2;
  while ((x /= 2) > 0);
}

void dump_bits() {
  for (size_t i = 0; i < sz; ++i) putchar(bits[i] + '0');
  putchar('\n');
}

uint64_t packet_version(char* p) {
  uint64_t version = 0;
  for (size_t i = 0; i < 3; ++i) version = version * 2 + p[i];
  return version;
}

uint64_t packet_type_id(char* p) {
  uint64_t type_id = 0;
  for (size_t i = 3; i < 6; ++i) type_id = type_id * 2 + p[i];
  return type_id;
}

uint64_t packet_literal(char* p) {
  assert(packet_type_id(p) == 4);
  uint64_t literal = 0;
  for (size_t i = 6;; i += 5) {
    for (size_t j = 1; j < 5; ++j) literal = literal * 2 + p[i + j];
    if (p[i] == 0) break;
  }
  return literal;
}

uint64_t packet_length_type_id(char* p) {
  assert(packet_type_id(p) != 4);
  return p[6];
}

uint64_t packet_length(char* p) {
  assert(packet_type_id(p) != 4);

  uint64_t length = 0;
  if (packet_length_type_id(p) == 0) {
    for (size_t i = 0; i < 15; ++i) length = length * 2 + p[7 + i];
  } else if (packet_length_type_id(p) == 1) {
    for (size_t i = 0; i < 11; ++i) length = length * 2 + p[7 + i];
  } else {
    assert(false);
  }
  return length;
}

char* packet_literal_end(char* b) {
  assert(packet_type_id(b) == 4);
  char* p = b + 6;
  while (true) {
    p += 5;
    if (p[-5] == 0) break;
  }
  return p;
}

char* packet_end(char* b) {
  uint64_t type_id = packet_type_id(b);
  if (type_id == 4) return packet_literal_end(b);

  uint64_t length_type_id = packet_length_type_id(b);
  uint64_t length = packet_length(b);
  if (length_type_id == 0) {
    char* p = b + 7 + lengths[length_type_id] + length;
    return p;
  }

  char* p = b + 7 + lengths[length_type_id];
  for (uint64_t i = 0; i < length; ++i) p = packet_end(p);
  return p;
}

uint64_t packet_subcount(char* p) {
  uint64_t count = 0;

  if (packet_type_id(p) != 4) {
    uint64_t length_type_id = packet_length_type_id(p);
    uint64_t length = packet_length(p);

    if (length_type_id == 1) return length;

    char* b = p + 7 + lengths[length_type_id];
    char* new_p = b;

    while ((uint64_t)(new_p - b) < length) {
      new_p = packet_end(new_p);
      ++count;
    }
  }

  return count;
}

void parse_versions(char* p, uint64_t* version) {
  *version += packet_version(p);
  uint64_t type_id = packet_type_id(p);

  if (type_id == 4) {
    return;
  } else {
    uint64_t length_type_id = packet_length_type_id(p);
    char* b = p + 7 + lengths[length_type_id];
    char* new_p = b;

    for (uint64_t i = 0; i < packet_subcount(p); ++i) {
      parse_versions(new_p, version);
      new_p = packet_end(new_p);
    }
  }
}

uint64_t parse_expr(char* p) {
  uint64_t type_id = packet_type_id(p);
  if (type_id == 4) return packet_literal(p);

  uint64_t length_type_id = packet_length_type_id(p);
  char* b = p + 7 + lengths[length_type_id];
  char* new_p = b;
  uint64_t count = packet_subcount(p);

  if (type_id == 0) {
    uint64_t res = 0;
    for (uint64_t i = 0; i < count; ++i) {
      res += parse_expr(new_p);
      new_p = packet_end(new_p);
    }
    return res;
  }

  if (type_id == 1) {
    uint64_t res = 1;
    for (uint64_t i = 0; i < count; ++i) {
      res *= parse_expr(new_p);
      new_p = packet_end(new_p);
    }
    return res;
  }

  if (type_id == 2) {
    uint64_t res = UINT64_MAX;
    for (uint64_t i = 0; i < count; ++i) {
      res = min(parse_expr(new_p), res);
      new_p = packet_end(new_p);
    }
    return res;
  }

  if (type_id == 3) {
    uint64_t res = 0;
    for (uint64_t i = 0; i < count; ++i) {
      res = max(parse_expr(new_p), res);
      new_p = packet_end(new_p);
    }
    return res;
  }

  if (type_id >= 5 || type_id <= 7) {
    uint64_t first = parse_expr(new_p);
    uint64_t second = parse_expr(packet_end(new_p));

    if (type_id == 5) return (uint64_t)(first > second);
    if (type_id == 6) return (uint64_t)(first < second);
    if (type_id == 7) return (uint64_t)(first == second);
  }

  assert(false);
}

int main(void) {
  bits = malloc(sizeof(*bits) * cap);

  char c = 0;
  while ((c = getchar()) != EOF) {
    while (cap < sz) cap *= 2;
    bits = realloc(bits, sizeof(*bits) * cap);

    append_bits(bits + sz, c);
    sz += 4;
  }
  bits = realloc(bits, sizeof(*bits) * sz);

  uint64_t version = 0;
  parse_versions(bits, &version);
  printf("%lu\n", version);
  printf("%lu\n", parse_expr(bits));

  free(bits);
}
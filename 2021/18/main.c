#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LB UINT64_MAX
#define RB (UINT64_MAX - 1lu)
#define CM (UINT64_MAX - 2lu)

uint64_t c2u64(char c) {
  if (c == '[') return LB;
  if (c == ']') return RB;
  if (c == ',') return CM;
  assert(false);
}

char u642c(uint64_t t) {
  if (t == LB) return '[';
  if (t == RB) return ']';
  if (t == CM) return ',';
  assert(false);
}

bool isnum(uint64_t t) { return t < CM; }

uint64_t entoken(char* p, char** end) {
  uint64_t t;

  if (isdigit(*p)) {
    return strtol(p, end, 10);
  } else {
    *end = p + 1;
    return c2u64(*p);
  }
}

size_t tokenize(char* p, uint64_t* a) {
  size_t i = 0;
  while (*p != '\0') a[i++] = entoken(p, &p);
  return i;
}

void dump(uint64_t* a, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    if (!isnum(a[i]))
      putchar(u642c(a[i]));
    else
      printf("%lu", a[i]);
  }
  puts("");
}

size_t insert(uint64_t* a, size_t sz, size_t pos, uint64_t t) {
  uint64_t tmp[512] = {0};

  size_t i = 0;
  for (size_t j = 0; j < sz; ++j) {
    if (j == pos) tmp[i++] = t;
    tmp[i++] = a[j];
  }

  memcpy(a, tmp, sizeof(uint64_t) * 512);
  return sz + 1;
}

size_t erase(uint64_t* a, size_t sz, size_t pos) {
  uint64_t tmp[512] = {0};

  size_t i = 0;
  for (size_t j = 0; j < sz; ++j) {
    if (j == pos) continue;
    tmp[i++] = a[j];
  }

  memcpy(a, tmp, sizeof(uint64_t) * 512);
  return sz - 1;
}

size_t add(uint64_t* c, uint64_t* a, size_t asz, uint64_t* b, size_t bsz) {
  uint64_t tmp[512] = {0};
  
  size_t i = 0;
  tmp[i++] = LB;
  for (size_t j = 0; j < asz; ++j) tmp[i++] = a[j];
  tmp[i++] = CM;
  for (size_t j = 0; j < bsz; ++j) tmp[i++] = b[j];
  tmp[i++] = RB;

  memcpy(c, tmp, sizeof(uint64_t) * 512);
  return i;
}

bool explode(uint64_t* a, size_t* sz) {
  size_t asz = *sz;
  int cnt = 0;

  for (size_t i = 0; i < asz; ++i) {
    if (a[i] == LB) {
      if (cnt < 4) {
        ++cnt;
      } else {
        // EXPLODE!!!
        int j = i - 1;
        while (j >= 0 && !isnum(a[j])) --j;
        if (j >= 0) a[j] += a[i + 1];
        j = i + 5;
        while (j < asz && !isnum(a[j])) ++j;
        if (j < asz) a[j] += a[i + 3];

        asz = erase(a, asz, i);
        asz = erase(a, asz, i);
        asz = erase(a, asz, i);
        asz = erase(a, asz, i);
        asz = erase(a, asz, i);
        asz = insert(a, asz, i, 0);

        *sz = asz;
        return true;
      }
    } else if (a[i] == RB) {
      --cnt;
    }
    assert(cnt >= 0);
  }

  return false;
}

bool split(uint64_t* a, size_t* sz) {
  size_t asz = *sz;

  for (size_t i = 0; i < asz; ++i) {
    if (a[i] > 9 && a[i] < CM) {
      // Split!
      int num = a[i];
      asz = erase(a, asz, i);
      asz = insert(a, asz, i, RB);
      uint64_t t;
      t = num / 2 + num % 2;
      asz = insert(a, asz, i, t);
      asz = insert(a, asz, i, CM);
      t = num / 2;
      asz = insert(a, asz, i, t);
      asz = insert(a, asz, i, LB);
      *sz = asz;
      return true;
    }
  }

  return false;
}


void divide(uint64_t* t, size_t sz, uint64_t* a, size_t* asz, uint64_t* b, size_t* bsz) {
  int cnt = 0;

  size_t i;
  for (i = 1; i < sz; ++i) {
    if (t[i] == LB)
      ++cnt;
    else if (t[i] == RB)
      --cnt;
    if (cnt == 0) break;
  }
  ++i;

  size_t c = i;
  for (size_t j = 1; j < c; ++j) a[j - 1] = t[j];
  *asz = c - 1;

  ++i;
  cnt = 0;
  for (; i < sz - 1; ++i) {
    if (t[i] == LB)
      ++cnt;
    else if (t[i] == RB)
      --cnt;
    if (cnt == 0) break;
  }
  ++i;

  for (size_t j = c + 1; j < i; ++j) b[j - c - 1] = t[j];
  *bsz = i - c - 1;
}

uint64_t magnitude(uint64_t* t, size_t sz) {
  if (sz == 1) return *t;

  uint64_t a[512] = {0};
  uint64_t b[512] = {0};
  size_t asz = 0;
  size_t bsz = 0;

  divide(t, sz, a, &asz, b, &bsz);

  return 3 * magnitude(a, asz) + 2 * magnitude(b, bsz);
}

void main1(void) {
  char buf[4096] = {0};
  uint64_t a[512];
  uint64_t b[512];
  size_t asz = 0;
  size_t bsz = 0;

  scanf("%s", buf);
  asz = tokenize(buf, a);

  while (scanf("%s", buf) == 1) {
    bsz = tokenize(buf, b);
    asz = add(a, a, asz, b, bsz);
    while (explode(a, &asz) || split(a, &asz))
      ;
  }

  printf("%lu\n", magnitude(a, asz));
}

void main2(void) {
  uint64_t tbl[256][512] = {0};
  size_t szs[256] = {0};
  size_t tbsz = 0;

  char buf[4096] = {0};
  while (scanf("%s", buf) == 1) {
    szs[tbsz] = tokenize(buf, tbl[tbsz]);
    ++tbsz;
  }

  uint64_t a[512];
  size_t asz = 0;

  uint64_t high = 0;
  for (size_t i = 0; i < tbsz; ++i) {
    for (size_t j = 0; j < tbsz; ++j) {
      if (i == j) continue;
      asz = add(a, tbl[i], szs[i], tbl[j], szs[j]);
      while (explode(a, &asz) || split(a, &asz))
        ;
      uint64_t mag = magnitude(a, asz);
      if (mag > high) high = mag;
    }
  }

  printf("%lu\n", high);
}

#if 0
int main(void) {
  main1();
}
#else
int main(void) {
  main2();
}
#endif
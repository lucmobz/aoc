#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SZ 256
#define LEN (sizeof(unsigned) * 8)

static char line[LINE_SZ];

int main(void) {
  size_t cap = 4096;
  size_t sz = 0;
  size_t len = 0;
  unsigned* nums = malloc(sizeof(unsigned) * cap);

  while (fgets(line, LINE_SZ, stdin)) {
    if (len == 0)
      while (isdigit(line[len])) ++len;

    if (cap <= sz) {
      while (cap <= sz) cap *= 2;
      nums = realloc(nums, sizeof(unsigned) * cap);
    }

    nums[sz++] = strtoul(line, NULL, 2);
  }

  size_t* cnts = malloc(sizeof(size_t) * len);

  for (size_t i = 0; i < sz; ++i) {
    unsigned num = nums[i];

    for (size_t j = 0; j < len; ++j)
      if ((num >> j) & 1u) ++cnts[j];
  }

  unsigned gamma = 0;
  unsigned epsilon = 0;

  for (size_t i = 0; i < len; ++i)
    if (cnts[i] >= sz - cnts[i]) gamma |= (1u << i);

  epsilon = ~gamma & ~(-1u << len);

  printf("gamma %u\nepsilon %u\n", gamma, epsilon);
  printf("%u\n", gamma * epsilon);

  size_t num_o2 = sz;
  size_t num_co2 = sz;

  bool* fo2 = malloc(sizeof(bool) * sz);
  bool* fco2 = malloc(sizeof(bool) * sz);

  memset(fo2, true, sizeof(bool) * sz);
  memset(fco2, true, sizeof(bool) * sz);

  for (size_t i = 0; i < len && (num_o2 > 1 || num_co2); ++i) {
    unsigned bit = 1u << (len - 1 - i);

    if (num_o2 > 1) {
      size_t cnt = 0;

      for (size_t j = 0; j < sz; ++j)
        if (fo2[j] && nums[j] & bit) ++cnt;

      unsigned expected_bit = 0;
      if (cnt >= num_o2 - cnt) expected_bit = bit;

      for (size_t j = 0; j < sz; ++j) {
        if (fo2[j] && (nums[j] & bit) != expected_bit) {
          fo2[j] = false;
          --num_o2;
        }
      }
    }

    if (num_co2 > 1) {
      size_t cnt = 0;

      for (size_t j = 0; j < sz; ++j)
        if (fco2[j] && nums[j] & bit) ++cnt;

      unsigned expected_bit = 0;
      if (cnt < num_co2 - cnt) expected_bit = bit;

      for (size_t j = 0; j < sz; ++j) {
        if (fco2[j] && (nums[j] & bit) != expected_bit) {
          fco2[j] = false;
          --num_co2;
        }
      }
    }
  }

  unsigned o2 = 0;
  unsigned co2 = 0;

  for (size_t i = 0; i < sz; ++i) {
    if (fo2[i]) {
      o2 = nums[i];
      printf("O2 %u\n", nums[i]);
    }
    if (fco2[i]) {
      co2 = nums[i];
      printf("CO2 %u\n", nums[i]);
    }
  }

  printf("%u\n", o2 * co2);

  free(nums);
  free(cnts);
  free(fo2);
  free(fco2);

  return 0;
}

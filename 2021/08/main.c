#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 0 1 2 3 4 5 6 7 8 9
// len 5: 2 5 3
// len 6: 0 6 9

static char words[10][8] = {0};
static char displays[4][8] = {0};
static char digits[10][8] = {0};

int lexcmp(const void* l, const void* r) {
  if (*(char*)l < *(char*)r)
    return -1;
  else if (*(char*)l > *(char*)r)
    return 1;
  else
    return 0;
}

void sort_str(char* str) { qsort(str, strlen(str), sizeof(char), lexcmp); }

void dump_digits() {
  for (size_t i = 0; i < 10; ++i)
    printf("%zu:%s%c", i, digits[i], i == 9 ? '\n' : ' ');
}

unsigned count_common(const char* str0, const char* str1) {
  unsigned common = 0;

  for (size_t i = 0; i < strlen(str0); ++i)
    for (size_t j = 0; j < strlen(str1); ++j)
      if (str0[i] == str1[j]) ++common;

  return common;
}

void decode0(const char* word) {
  size_t len = strlen(word);

  if (len == 2) {
    strcpy(digits[1], word);
  } else if (len == 7) {
    strcpy(digits[8], word);
  } else if (len == 3) {
    strcpy(digits[7], word);
  } else if (len == 4) {
    strcpy(digits[4], word);
  }
}

void decode1(const char* word) {
  size_t len = strlen(word);

  if (len == 5) {
    unsigned common1 = count_common(word, digits[1]);

    if (common1 == 2) {
      strcpy(digits[3], word);
    } else {
      unsigned common4 = count_common(word, digits[4]);

      if (common4 == 2)
        strcpy(digits[2], word);
      else
        strcpy(digits[5], word);
    }
  } else if (len == 6) {
    unsigned common1 = count_common(word, digits[1]);

    if (common1 == 1) {
      strcpy(digits[6], word);
    } else {
      unsigned common4 = count_common(word, digits[4]);

      if (common4 == 3)
        strcpy(digits[0], word);
      else
        strcpy(digits[9], word);
    }
  }
}

char decode2(const char* word) {
  for (size_t i = 0; i < 10; ++i)
    if (!strcmp(word, digits[i])) return (char)i + '0';
  return 'x';
}

int main(void) {
  uint64_t count = 0;

  while (!feof(stdin)) {
    memset(digits, 0, sizeof(char) * 10 * 8);
    memset(words, 0, sizeof(char) * 10 * 8);
    memset(displays, 0, sizeof(char) * 4 * 8);

    puts("Scanning part 1:");

    for (size_t i = 0; i < 10; ++i) {
      scanf("%s", words[i]);
      sort_str(words[i]);
    }

    for (size_t i = 0; i < 10; ++i)
      printf("%s%c", words[i], i == 9 ? '\n' : ' ');
    dump_digits();

    for (size_t i = 0; i < 10; ++i) decode0(words[i]);
    dump_digits();

    for (size_t i = 0; i < 10; ++i) decode1(words[i]);
    dump_digits();

    scanf(" | ");

    puts("Scanning part 2:");
    for (size_t i = 0; i < 4; ++i) {
      scanf("%s", displays[i]);
      sort_str(displays[i]);
      printf("%s%c", displays[i], i == 3 ? '\n' : ' ');
    }

    char str[5] = {0};
    for (size_t i = 0; i < 4; ++i) 
      str[i] = decode2(displays[i]);
    
    count += strtoul(str, NULL, 10);

    puts(str);
    char c = getchar();
    printf("DEBUG:%d|", (int)c);
    ungetc(c, stdin);
    puts("");
  }

  printf("%lu\n", count);
}

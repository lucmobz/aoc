#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ROUND_COUNT
#define ROUND_COUNT 20
#endif
#define ITEM_COUNT 64
#define LINE_SIZE 256
#ifndef MONKEY_COUNT
#define MONKEY_COUNT 8
#endif
#define OPERATION_SIZE 32
#ifndef PART
#define PART 1
#endif

typedef uint64_t u64;

typedef struct {
  u64 items[ITEM_COUNT];
  size_t item_count;
  char operation[OPERATION_SIZE];
  u64 test;
  size_t true_next;
  size_t false_next;
  u64 count;
} Monkey;

void parse_monkeys(Monkey *monkeys) {
  char line[LINE_SIZE] = {0};

  for (size_t i = 0; i < MONKEY_COUNT; ++i) {
    fgets(line, LINE_SIZE, stdin);

    // Parse items
    fgets(line, LINE_SIZE, stdin);
    char *ptr = line;
    while (*ptr != '\n') {
      while (*ptr != '\n' && !isdigit(*ptr))
        ++ptr;
      if (isdigit(*ptr))
        monkeys[i].items[monkeys[i].item_count++] = strtoull(ptr, &ptr, 10);
    }

    // Parse operation
    fgets(line, LINE_SIZE, stdin);
    ptr = line + 19; // new = $
    size_t len = strlen(ptr) - 1;
    strncpy(monkeys[i].operation, ptr, len);

    // Parse test
    fgets(line, LINE_SIZE, stdin);
    monkeys[i].test = strtoull(line + 20, NULL, 10);
    fgets(line, LINE_SIZE, stdin);
    monkeys[i].true_next = strtoull(line + 28, NULL, 10);
    fgets(line, LINE_SIZE, stdin);
    monkeys[i].false_next = strtoull(line + 29, NULL, 10);

    fgets(line, LINE_SIZE, stdin);
  }
}

void print_monkeys(const Monkey *monkeys, size_t round) {
  printf("Round %zu\n", round);
  for (size_t i = 0; i < MONKEY_COUNT; ++i) {
    printf("Monkey %zu: ", i);
    for (size_t j = 0; j < monkeys[i].item_count; ++j) {
      printf("%lu ", monkeys[i].items[j]);
    }
    putchar('\n');
  }
  putchar('\n');
}

u64 perform_operation(char *op, u64 old) {
  char c = op[4];
  op += 6;
  u64 rhs = op[0] == 'o' ? old : strtoull(op, NULL, 10);
  return c == '+' ? old + rhs : old * rhs;
}

int main(void) {
  Monkey monkeys[MONKEY_COUNT] = {0};
  parse_monkeys(monkeys);

  // Used for part 2
  u64 magic_number = 1;
  for (size_t i = 0; i < MONKEY_COUNT; ++i)
    magic_number *= monkeys[i].test;

  for (size_t round = 0; round < ROUND_COUNT; ++round) {
    // print_monkeys(monkeys, round);

    for (size_t i = 0; i < MONKEY_COUNT; ++i) {
      for (size_t j = 0; j < monkeys[i].item_count; ++j) {
        // Stack pop
        u64 item = monkeys[i].items[j];
        monkeys[i].items[j] = 0;

        ++monkeys[i].count;
        item = perform_operation(monkeys[i].operation, item);

        if (PART == 1)
          item /= 3;
        else
          item %= magic_number;

        size_t next = (item % monkeys[i].test == 0) ? monkeys[i].true_next
                                                    : monkeys[i].false_next;
        assert(next != i);
        assert(next < MONKEY_COUNT);
        assert(monkeys[next].item_count < ITEM_COUNT);
        // Stack push
        monkeys[next].items[monkeys[next].item_count++] = item;
      }
      // Reset items
      monkeys[i].item_count = 0;
    }
  }
  // print_monkeys(monkeys, ROUND_COUNT);

  u64 first = 0;
  u64 second = 0;
  for (size_t i = 0; i < MONKEY_COUNT; ++i) {
    u64 count = monkeys[i].count;
    if (count > first) {
      second = first;
      first = count;
    } else if (count > second) {
      second = count;
    }
  }

  printf("Part %d: %lu\n", PART, first * second);
}

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int64_t regs[4] = {0};

void dump_regs() {
  for (size_t i = 0; i < 4; ++i) {
    printf("%ld ", regs[i]);
  }
}

typedef struct Instruction {
  int64_t c;
  size_t a;
  size_t b;
  unsigned char type;
} Instruction;

enum { INP, ADD, MUL, DIV, MOD, EQL };

char* type2str(unsigned char type) {
  static char str[4];
  memset(str, 0, sizeof(char) * 4);

  switch (type) {
    case INP:
      strcpy(str, "inp");
      break;
    case ADD:
      strcpy(str, "add");
      break;
    case MUL:
      strcpy(str, "mul");
      break;
    case DIV:
      strcpy(str, "div");
      break;
    case MOD:
      strcpy(str, "mod");
      break;
    case EQL:
      strcpy(str, "eql");
      break;
  }

  return str;
}

unsigned char str2type(char* str) {
  if (!strncmp(str, "inp", 3)) return INP;
  if (!strncmp(str, "add", 3)) return ADD;
  if (!strncmp(str, "mul", 3)) return MUL;
  if (!strncmp(str, "div", 3)) return DIV;
  if (!strncmp(str, "mod", 3)) return MOD;
  if (!strncmp(str, "eql", 3)) return EQL;
  return -1;
}

static Instruction program[4096] = {0};
static size_t sz = 0;

Instruction make_instruction(char* line) {
  Instruction in;

  in.type = str2type(line);

  char* p = line + 4;
  in.a = *p - 'w';

  p = line + 6;
  if (isalpha(*p)) {
    in.b = *p - 'w';
  } else {
    in.b = 4;
    in.c = strtoll(p, NULL, 10);
  }

  return in;
}

void dump_instruction(Instruction in, bool verbose) {
  if (verbose) dump_regs();
  printf("%s %c", type2str(in.type), (char)in.a + 'w');
  if (in.type != INP) {
    if (in.b == 4) {
      printf(" %ld", in.c);
    } else {
      printf(" %c", (char)in.b + 'w');
    }
  }
}

void dump_program(Instruction* p, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    dump_instruction(p[i], false);
    putchar('\n');
  }
}

int64_t run(const char* digits, Instruction* p, size_t sz) {
  memset(regs, 0, sizeof(int64_t) * 4);

  size_t d = 0;

  for (size_t i = 0; i < sz; ++i) {
    Instruction in = p[i];
    int64_t b = in.b == 4 ? in.c : regs[in.b];
    int64_t* a = &regs[in.a];

    switch (in.type) {
      case INP:
        *a = digits[d++];
        break;
      case ADD:
        *a += b;
        break;
      case MUL:
        *a *= b;
        break;
      case DIV:
        *a /= b;
        break;
      case MOD:
        *a = *a % b;
        break;
      case EQL:
        *a = *a == b ? 1 : 0;
    }
  }

  return regs[3];
}

#define READ (INT64_MAX - 1)
#define UNKNOWN INT64_MAX

size_t optimize(Instruction* o, Instruction* p, size_t sz, bool verbose) {
  memset(regs, 0, sizeof(int64_t) * 4);
  size_t osz = 0;
  bool skipped[4096] = {0};

  for (size_t i = 0; i < sz; ++i) {
    Instruction in = p[i];
    bool skip = false;
    int64_t b = in.b < 4 ? regs[in.b] : in.c;
    int64_t* a = &regs[in.a];

    if (in.type == INP) {
      *a = READ;
    } else {
      switch (in.type) {
        case ADD:
          if (b == 0)
            skip = true;
          else if (*a == 0 && b == READ) 
            *a = READ;
          else if (*a >= READ || b >= READ)
            *a = UNKNOWN;
          else
            *a += b;

          break;

        case MUL:
          if (b == 1)
            skip = true;
          else if (*a == 0)
            skip = true;
          else if (*a == 0 || b == 0)
            *a = 0;
          else if (*a == 1 && b == READ) 
            *a = READ;
          else if (*a >= READ || b >= READ)
            *a = UNKNOWN;
          else
            *a *= b;

          break;

        case DIV:
          if (b == 1)
            skip = true;
          else if (*a >= READ || b >= READ)
            *a = UNKNOWN;
          else
            *a /= b;
          break;

        case MOD:
          if (*a == 0)
            skip = true;
          else
            *a = *a % b;
          break;

        case EQL:
          if (*a == READ && b < READ && (b <= 0 || b >= 10))
            *a = 0;
          else if (b == READ && *a < READ && (*a <= 0 || *a >= 10))
            *a = 0;
          else if (*a >= READ || b >= READ)
            *a = UNKNOWN;
          else
            *a = *a == b ? 1 : 0;
          break;
      }
    }

    if (skip) skipped[i] = true;

    if (verbose) {
      printf(skip ? "[x] " : "[ ] ");
      dump_instruction(p[i], verbose);
      putchar('\n');
    }
  }

  for (size_t i = 0; i < sz; ++i) if (!skipped[i]) o[osz++] = p[i];

  return osz;
}

int main(void) {
  char line[64] = {0};
  while (fgets(line, 64, stdin)) program[sz++] = make_instruction(line);
  // dump_program(program, sz);

  Instruction o[4096];
  size_t osz = optimize(o, program, sz, false);
  //printf("%zu/%zu\n", osz, sz);
  osz = optimize(o, o, osz, false);
  //printf("%zu/%zu\n", osz, sz);

  //int64_t n = powl(10, 14) - 1;
  int64_t n = 20000000000000;
  int64_t lower = powl(10, 13) - 1;

  while (true) {
    if (n <= lower) break;
    char str[15] = {0};
    snprintf(str, 14, "%ld", n);

    for (size_t i = 0; i < 14; ++i) {
      if (str[i] == '0') {
        --n;
        continue;
      }
    }

    if (run(str, o, osz) == 0) {
      puts(str);
      break;
    }
    
    if (n % 100000 == 0) printf("%ld\n", n);
    --n;
  }

  dump_program(o, osz);
  return osz;
}
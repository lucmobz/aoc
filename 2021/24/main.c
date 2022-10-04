#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void dump_instruction(Instruction in) {
  dump_regs();
  printf("%s %c", type2str(in.type), (char)in.a + 'w');
  if (in.type != INP) {
    if (in.b == 4) {
      printf(" %ld", in.c);
    } else {
      printf(" %c", (char)in.b + 'w');
    }
  }
  putchar('\n');
}

void dump_program(Instruction* p, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    dump_instruction(p[i]);
  }
}

int64_t run(const char* digits, Instruction* p, size_t sz) {
  memset(regs, 0, sizeof(int64_t) * 4);

  size_t d = 0;

  for (size_t i = 0; i < sz; ++i) {
    Instruction in = p[i];
    int64_t b = in.b == 4 ? in.c : regs[in.b];

    switch (in.type) {
      case INP:
        regs[in.a] = digits[d++];
        break;
      case ADD:
        regs[in.a] += b;
        break;
      case MUL:
        regs[in.a] *= b;
        break;
      case DIV:
        regs[in.a] /= b;
        break;
      case MOD:
        regs[in.a] = regs[in.a] % b;
        break;
      case EQL:
        regs[in.a] = regs[in.a] == b ? 1 : 0;
    }
  }

  return regs[3];
}

size_t optimize(Instruction* o, Instruction* p, size_t sz) {
  memset(regs, 0, sizeof(int64_t) * 4);
  size_t osz = 0;

  for (size_t i = 0; i < sz; ++i) {
    Instruction in = p[i];
    bool skip = false;
    int64_t b = in.b < 4 ? regs[in.b] : in.c;

    if (in.type == INP) {
      regs[in.a] = -2;
    } else if (in.type != INP && in.b < 4 && regs[in.b] < 0) {
      regs[in.a] = regs[in.b];
    } else {
      switch (in.type) {
        case ADD:
          if (in.b == 4 && in.c == 0)
            skip = true;
          else if (regs[in.a] == 0 && in.b < 4 && regs[in.b] == 0)
            skip = true;
          else if (regs[in.a] == 0 && in.b == 4 && in.c == 0)
            skip = true;
          else if (regs[in.a] < 0 && in.b == 4)
            regs[in.a] = in.c;
          break;

        case MUL:
          if (regs[in.a] == 0 && in.b == 4 && in.c == 0)
            skip = true;
          else if (in.b == 4 && in.c == 0)
            regs[in.a] = 0;
          break;

        case DIV:
          if (in.b == 4 && in.c == 1) skip = true;
          break;

        case MOD:
          if (regs[in.a] == 0) skip = true;
          break;

        case EQL:
          if (regs[in.a] < 0 && in.b == 4)
            regs[in.a] = regs[in.a] == in.c ? 1 : 0;
          else if (regs[in.a] == -2 && in.b == 4 && (in.c > 10 || in.c <= 0)) regs[in.a] == 0;
          else if (regs[in.a] == -2 && in.b < 4 && (regs[in.b] > 10 || in.c <= 0)) regs[in.a] == 0;
      }
    }

    if (!skip){
      o[osz++] = in;
      dump_instruction(in);
    }

  }

  return osz;
}

int main(void) {
  char line[64] = {0};
  while (fgets(line, 64, stdin)) program[sz++] = make_instruction(line);
  // dump_program(program, sz);

  Instruction o[4096];
  size_t osz = optimize(o, program, sz);
  printf("%zu/%zu\n", osz, sz);

  int64_t n = 99999999999999;

  while (false) {
    if (n <= 9999999999999) break;
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

    --n;
  }

  // dump_program(o, osz);
}
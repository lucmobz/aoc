#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 16

static char line[LINE_SIZE];

int main(void) {

  unsigned count = 0;

  while (fgets(line, LINE_SIZE, stdin)) {
    char *ptr = line;

    int a = strtol(ptr, &ptr, 10);
    int b = strtol(ptr + 1, &ptr, 10);
    int c = strtol(ptr + 1, &ptr, 10);
    int d = strtol(ptr + 1, &ptr, 10);

    if ((a <= c && d <= b) || (c <= a && b <= d))
      ++count;
  }

  printf("Part 1:\n%d\n", count);
}

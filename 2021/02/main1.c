#include <stdio.h>

int main(void) {
  int depth = 0;
  int horiz = 0;

  char dir[64] = {0};
  int val = 0;

  while(fscanf(stdin, "%s %d\n", dir, &val) == 2) {
    switch (dir[0]) {
      case 'f':
        horiz += val;
        break;

      case 'd':
        depth += val;
        break;

      case 'u':
        depth -= val;
        if (depth < 0) {
          puts("We resurfaced!");
          depth = 0;
        }
        break;

      default:
        fprintf(stderr, "ERROR: unexpected direction\n");
        return 1;
    }
  }

  printf("Part 1: %d\n", depth * horiz);

  return 0;
}

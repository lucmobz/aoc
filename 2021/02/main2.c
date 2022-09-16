#include <stdio.h>

int main(void) {
  int depth = 0;
  int horiz = 0;
  int aim = 0;

  char dir[256] = {0};
  int val = 0;

  while (fscanf(stdin, "%s %d\n", dir, &val) == 2) {
    switch (dir[0]) {
      case 'f':
        horiz += val;
        depth += aim * val;
        break;

      case 'd':
        aim += val;
        break;

      case 'u':
        aim -= val;
        break;

      default:
        fprintf(stderr, "ERROR: unexpected direction\n");
        return 1;
    }
  }

  printf("%d\n", depth * horiz);

  return 0;
}

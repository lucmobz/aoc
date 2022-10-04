#include <limits.h>
#include <stdio.h>

int main(int argc, char** argv) {
  if (argc > 1) {
    fprintf(stderr, "USAGE: %s < input.txt \n", argv[0]);
    return 1;
  }

  int prev = INT_MAX;
  int curr = 0;
  int count = 0;

  while (fscanf(stdin, "%d\n", &curr) == 1) {
    if (curr > prev) ++count;
    prev = curr;
  }

  printf("%d\n", count);

  return 0;
}

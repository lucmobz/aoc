#include <stdio.h>

int main(void) {
  int num0 = 0;
  int num1 = 0;
  int num2 = 0;
  int sum0 = 0;
  int num3 = 0;
  int count = 0;
  
  fscanf(stdin, "%d\n", &num0);
  fscanf(stdin, "%d\n", &num1);
  fscanf(stdin, "%d\n", &num2);

  sum0 = num0 + num1 + num2;

  while (fscanf(stdin, "%d\n", &num3) == 1) {
    int sum1 = sum0 - num0 + num3;

    if (sum1 > sum0) ++count;

    sum0 = sum1;
    num0 = num1;
    num1 = num2;
    num2 = num3;
  }
 
  printf("Part 2: %d\n", count);

  return 0;
}

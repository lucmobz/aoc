#include <stdint.h>
#include <stdio.h>

int64_t n1[] = {15, 16, 4, 14, 1, 3, 5};
int64_t n2[] = {8, 10, 3, 4, 5, 8, 11};

int main(void) {
  for (int64_t w0 = 9; w0 >= 1; --w0) {
    for (int64_t w1 = 9; w1 >= 1; --w1) {
      for (int64_t w2 = 9; w2 >= 1; --w2) {
        for (int64_t w3 = 9; w3 >= 1; --w3) {
          int64_t z3 =
              (((w0 + n1[0]) * 26 + w1 + n1[1]) * 26 + w2 + n1[2]) * 26 + w3 +
              n1[3];
          int64_t w4 = z3 % 26 - n2[0];
          if (w4 <= 0 || w4 >= 10) continue;
          z3 /= 26;
          int64_t w5 = z3 % 26 - n2[1];
          if (w5 <= 0 || w5 >= 10) continue;
          z3 /= 26;
          for (int64_t w6 = 9; w6 >= 1; --w6) {
            int64_t z6 = z3 * 26 + w6 + n1[4];
            int64_t w7 = z6 % 26 - n2[2];
            if (w7 <= 0 || w7 >= 10) continue;
            z6 /= 26;
            for (int64_t w8 = 9; w8 >= 1; --w8) {
              int64_t z8 = z6 * 26 + w8 + n1[5];
              int64_t w9 = z8 % 26 - n2[3];
              if (w9 <= 0 || w9 >= 10) continue;
              z8 /= 26;
              for (int64_t w10 = 9; w10 >= 1; --w10) {
                int64_t z10 = z8 * 26 + w10 + n1[6];
                int64_t w11 = z10 % 26 - n2[4];
                if (w11 <= 0 || w11 >= 10) continue;
                z10 /= 26;
                int64_t w12 = z10 % 26 - n2[5];
                if (w12 <= 0 || w12 >= 10) continue;
                z10 /= 26;
                int64_t w13 = z10 % 26 - n2[6];
                if (w13 <= 0 || w13 >= 10) continue;
                z10 /= 26;

                if (z10 == 0) {
                  printf(
                      "%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%"
                      "ld\n",
                      w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12,
                      w13);
                }
              }
            }
          }
        }
      }
    }
  }
}
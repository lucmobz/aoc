#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SIGNAL_SIZE 4095 // Take care of possible newline and zero terminator
#ifndef WINDOW_SIZE
#define WINDOW_SIZE 4
#endif
#ifndef PART
#define PART 1
#endif

static char signal[SIGNAL_SIZE + 2];

int main(void) {
  fgets(signal, SIGNAL_SIZE + 2, stdin);

  const char *window = signal;
  size_t pos = SIGNAL_SIZE;

  while (window + WINDOW_SIZE <= signal + SIGNAL_SIZE) {
    bool reset = false;

    for (size_t i = 0; i < WINDOW_SIZE - 1; ++i) {
      for (size_t j = i + 1; j < WINDOW_SIZE; ++j) {
        if (window[i] == window[j]) {
          reset = true;
          break;
        }
      }
      if (reset) {
        window += i + 1; // Skip forward by more than one
        break;
      }
    }

    if (!reset) {
      pos = (window - signal) + WINDOW_SIZE;
      break;
    }
  }

  printf("Part %d: %zu\n", PART, pos);

  if (pos == SIGNAL_SIZE)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}

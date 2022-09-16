#include <stdint.h>
#include <stdio.h>

#define SPAWN_DAYS 9
#define RUN_DAYS 256

static uint64_t fishes[SPAWN_DAYS];

void update(uint64_t* cnts) {
  uint64_t cnt0 = cnts[0];

  for (size_t i = 1; i < SPAWN_DAYS; ++i) 
    cnts[i - 1] = cnts[i];
  
  cnts[SPAWN_DAYS - 1] = cnt0;
  cnts[SPAWN_DAYS - 1 - 2] += cnt0;
}

int main(void) {
  size_t value;

  while (scanf("%zu,", &value) == 1) ++fishes[value];
  
  for (size_t i = 0; i < RUN_DAYS; ++i)
    update(fishes);

  uint64_t count = 0;
  for (size_t i = 0; i < SPAWN_DAYS; ++i) count += fishes[i];
  
  uint64_t days = RUN_DAYS;
  printf("Days: %lu\nFishes: %lu\n", days, count);
}

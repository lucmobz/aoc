#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINGO_SZ 4096

static char bingo[BINGO_SZ];

static int (*boards)[25];
static size_t boards_cap;
static size_t boards_sz;

static inline void print_board(int* board) {
  for (size_t i = 0; i < 25; ++i)
    printf("%3d%c", board[i], ((i + 1) % 5 == 0) ? '\n' : ' ');
}

static inline bool read_board() {
  if (feof(stdin)) return false;

  if (boards_cap <= boards_sz) {
    if (boards_cap == 0) boards_cap = 1;
    while (boards_cap <= boards_sz) boards_cap *= 2;
    boards = realloc(boards, sizeof(*boards) * boards_cap);
  }

  for (size_t i = 0; i < 25; ++i) {
    if (scanf("%d", &boards[boards_sz][i]) != 1) return false;
  }

  ++boards_sz;

  return true;
}

bool check_board(int* board) {
  for (size_t i = 0; i < 5; ++i) {
    int rows = 0;
    int cols = 0;

    for (size_t j = 0; j < 5; ++j) {
      if (board[j + i * 5] < 0) ++rows;
      if (board[i + j * 5] < 0) ++cols;
    }

    if (rows >= 5) return true;
    if (cols >= 5) return true;
  }

  return false;
}

void update_board(int* board, int num) {
  for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      int* entry = &board[j + i * 5];

      if (*entry == num) {
        *entry = -1 - *entry;
        return;
      }
    }
  }
}

void print_answer_from_board(int* board, int num) {
  int sum = 0;

  for (size_t i = 0; i < 25; ++i)
    if (board[i] >= 0) sum += board[i];

  printf("bingo %d\n%d\n", num, num * sum);
}

int main(void) {
  fgets(bingo, sizeof(bingo) / sizeof(char), stdin);

  while (read_board())
    ;

  boards = realloc(boards, sizeof(*boards) * boards_sz);

  bool* is_board_active = malloc(sizeof(bool) * boards_sz);
  memset(is_board_active, true, sizeof(bool) * boards_sz);

  int num = -1;
  size_t count = 0;

  char* str = bingo;
  while (*str != '\n' && !isdigit(*str)) ++str;

  while (*str != '\n') {
    num = strtol(str, &str, 10);
    
    for (size_t i = 0; i < boards_sz; ++i) {
      update_board(boards[i], num);

      // Set to false for part 1 of algorithm
      if (true) {
        if (count >= 5 && is_board_active[i] && check_board(boards[i])) {
          print_answer_from_board(boards[i], num); 
          is_board_active[i] = false;
        }
      } else {
        if (count >= 5 && check_board(boards[i])) {
          print_answer_from_board(boards[i], num); 
          return 0;
        }
      }
    }

    ++count;

    while (*str != '\n' && !isdigit(*str)) ++str;
  }

  free(boards);
  free(is_board_active);
}

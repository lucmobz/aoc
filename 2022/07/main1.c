#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 64
#define NAME_SIZE 16
#define SUBDIR_COUNT 32
#define DIR_COUNT 512

static char line_buf[LINE_SIZE] = {0};

typedef struct {
  char name[NAME_SIZE];
  unsigned long long size;
  size_t parent_dir;
  size_t subdirs[SUBDIR_COUNT];
  size_t subdir_count;
} Dir;

static Dir dirs[DIR_COUNT] = {0};
static size_t dir_count = 0;

void new_dir(size_t i, const char *name) {
  assert(dir_count < DIR_COUNT);
  strcpy(dirs[i].name, name);
}

size_t find_subdir(size_t dir, const char *name) {
  size_t i = 0;

  for (; i < dirs[dir].subdir_count; ++i) {
    size_t j = dirs[dir].subdirs[i];
    if (strcmp(dirs[j].name, name) == 0)
      break;
  }

  return i;
}

void append_subdir(size_t dir, size_t subdir) {
  assert(dirs[dir].subdir_count < SUBDIR_COUNT);
  dirs[dir].subdirs[dirs[dir].subdir_count++] = subdir;
}

void dump_dirs(size_t curr_dir, int level) {
  printf("%*s %llu\n", level * 4, dirs[curr_dir].name, dirs[curr_dir].size);
  for (size_t i = 0; i < dirs[curr_dir].subdir_count; ++i)
    dump_dirs(dirs[curr_dir].subdirs[i], level + 1);
}

unsigned long long process_sizes(size_t dir) {
  for (size_t i = 0; i < dirs[dir].subdir_count; ++i)
    dirs[dir].size += process_sizes(dirs[dir].subdirs[i]);
  return dirs[dir].size;
}

char *read_line(char *line) {
  char *res = fgets(line, LINE_SIZE, stdin);
  size_t len = strlen(line);
  if (line[len - 1] == '\n')
    line[len - 1] = 0;
  return res;
}

int main(void) {
  new_dir(0, "/");
  ++dir_count;
  size_t curr_dir = 0;
  char *line = read_line(line_buf);

  while (line) {
    // Interprete line
    if (line[0] == '$') {
      if (line[2] == 'c') {
        if (line[5] == '/') {
          curr_dir = 0;
        } else if (line[5] == '.') {
          curr_dir = dirs[curr_dir].parent_dir;
        } else {
          const char *name = line + 5;
          size_t subdir = find_subdir(curr_dir, name);

          if (subdir >= dirs[curr_dir].subdir_count) {
            new_dir(dir_count, name);
            dirs[dir_count].parent_dir = curr_dir;
            append_subdir(curr_dir, dir_count);
            curr_dir = dir_count;
            ++dir_count;
          } else {
            curr_dir = dirs[curr_dir].subdirs[subdir];
          }
        }
      }

      line = read_line(line);
    } else {
      unsigned long long size = 0;

      do {
        if (line[0] == 'd') {
          const char *name = line + 4;
          size_t subdir = find_subdir(curr_dir, name);

          if (subdir >= dirs[curr_dir].subdir_count) {
            new_dir(dir_count, name);
            dirs[dir_count].parent_dir = curr_dir;
            append_subdir(curr_dir, dir_count);
            ++dir_count;
          }
        } else {
          size += strtoull(line, NULL, 10);
        }

        line = read_line(line);
      } while (line && line[0] != '$');

      dirs[curr_dir].size = size;
    }
  }

  // Process sizes
  process_sizes(0);

  unsigned long long sum_sizes = 0;
  for (size_t i = 0; i < DIR_COUNT; ++i) {
    if (dirs[i].size <= 100000)
      sum_sizes += dirs[i].size;
  }

  printf("Part 1: %llu\n", sum_sizes);

  // dump_dirs(0, 0);

  assert(dirs[0].size <= 70000000);
  unsigned long long free_space = 70000000 - dirs[0].size;
  unsigned long long min_size = dirs[0].size;

  for (size_t i = 0; i < dir_count; ++i) {
    unsigned long long space = free_space + dirs[i].size;
    if (space >= 30000000 && space < min_size + free_space) {
      min_size = dirs[i].size;
    }
  }

  printf("Part 2: %llu\n", min_size);
}

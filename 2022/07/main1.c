#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 64
#define PATH_SIZE 512

static char line[LINE_SIZE] = {0};
static char workdir[PATH_SIZE] = {0};
static unsigned long long workdir_size = 0;

int main(void) {
  bool need_dumping = false;

  while (fgets(line, LINE_SIZE, stdin)) {
    if (line[0] == '$') {
      // Dump info
      if (need_dumping) {
        printf("%llu\n", workdir_size);
        need_dumping = false;
      }

      // Parse command
      if (line[2] == 'c') {
        if (line[5] == '/') {
          memset(workdir, 0, sizeof(char) * PATH_SIZE);
          strcpy(workdir, "/");
        } else if (line[5] == '.') {
          size_t end = strlen(workdir);
          if (end > 1) {
            // Delete backslash
            workdir[end - 1] = 0;
            --end;
          }
          while (end > 1 && workdir[end - 1] != '/') {
            workdir[end - 1] = 0;
            --end;
          }
        } else {
          // Skip newline
          strncpy(workdir + strlen(workdir), line + 5, strlen(line + 5) - 1);
          workdir[strlen(workdir)] = '/';
          // Prepare to parse output
          workdir_size = 0;
        }
      } else if (line[2] == 'l') {
        puts(workdir);
      }
    } else {
      // Parse output
      if (isdigit(line[0])) {
        workdir_size += strtoull(line, NULL, 10);
        need_dumping = true;
      }
    }
  }
}

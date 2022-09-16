#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char tag[64];
  size_t id;
  size_t* nb;
  size_t nb_sz;
} Node;

Node* graph = NULL;
size_t graph_sz = 0;

size_t find_or_append(const char* tag) {
  for (size_t i = 0; i < graph_sz; ++i) {
    if (!strcmp(graph[i].tag, tag)) return i;
  }

  ++graph_sz;
  graph = realloc(graph, graph_sz * sizeof(Node));

  strcpy(graph[graph_sz - 1].tag, tag);
  graph[graph_sz - 1].id = graph_sz - 1;
  graph[graph_sz - 1].nb = NULL;
  graph[graph_sz - 1].nb_sz = 0;

  return graph_sz - 1;
}

void dump_graph() {
  printf("Size: %zu\n", graph_sz);

  for (size_t i = 0; i < graph_sz; ++i) {
    Node* n = &graph[i];
    printf("(%zu)%s: ", n->id, n->tag);
    for (size_t j = 0; j < n->nb_sz; ++j)
      printf("(%zu)%s ", n->nb[j], graph[n->nb[j]].tag);
    printf("\n");
  }
}

void append_neighbor(Node* n, size_t i) {
  n->nb_sz += 1;
  n->nb = realloc(n->nb, sizeof(size_t) * n->nb_sz);
  n->nb[n->nb_sz - 1] = i;
}

bool find_in_stack(size_t id, size_t* stack, size_t stack_sz) {
  for (size_t i = 0; i < stack_sz; ++i)
    if (id == stack[i]) return true;
  return false;
}

int count_paths(size_t id, bool twice) {
  static int count = 0;
  static size_t stack[256] = {0};
  static size_t stack_sz = 0;

  if (!strcmp(graph[id].tag, "end")) {
    for (size_t i = 0; i < stack_sz; ++i) printf("%s,", graph[stack[i]].tag);
    printf("end\n");
    return ++count;
  }

  stack[stack_sz++] = id;

  for (size_t i = 0; i < graph[id].nb_sz; ++i) {
    size_t nb_id = graph[id].nb[i];

    if (isupper(graph[nb_id].tag[0]) ||
        !find_in_stack(nb_id, stack, stack_sz)) {
      count_paths(nb_id, twice);
    } else if (!twice && strcmp(graph[nb_id].tag, "start")) {
      // Comment next line for part 1
      count_paths(nb_id, true);
    }
  }

  --stack_sz;
  return count;
}

int main(void) {
  char line[64];

  while (fgets(line, 64, stdin)) {
    char* p1 = strtok(line, "-\n");
    char* p2 = strtok(NULL, "-\n");

    size_t id1 = find_or_append(p1);
    size_t id2 = find_or_append(p2);

    append_neighbor(&graph[id1], id2);
    append_neighbor(&graph[id2], id1);
  }

  size_t src = find_or_append("start");

  int count = count_paths(src, false);

  printf("%d\n", count);
}

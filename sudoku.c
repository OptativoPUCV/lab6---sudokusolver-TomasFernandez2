#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
   int sudo[9][9];
} Node;

Node* createNode() {
  Node* n = (Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n) {
    Node* new = (Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file(char* file_name) {
  Node* n = createNode();
  FILE* file = fopen(file_name, "r");
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (!fscanf(file, "%d", &n->sudo[i][j]))
        printf("failed to read data!");
    }
  }
  fclose(file);
  return n;
}

void print_node(Node* n) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++)
      printf("%d ", n->sudo[i][j]);
    printf("\n");
  }
  printf("\n");
}

int is_valid(Node* n) {
  int i, j, k, l;
  int nums[10];

  // Check rows
  for (i = 0; i < 9; i++) {
    for (k = 0; k <= 9; k++) nums[k] = 0;
    for (j = 0; j < 9; j++) {
      int num = n->sudo[i][j];
      if (num != 0) {
        if (nums[num] == 1) return 0;
        nums[num] = 1;
      }
    }
  }

  // Check columns
  for (j = 0; j < 9; j++) {
    for (k = 0; k <= 9; k++) nums[k] = 0;
    for (i = 0; i < 9; i++) {
      int num = n->sudo[i][j];
      if (num != 0) {
        if (nums[num] == 1) return 0;
        nums[num] = 1;
      }
    }
  }

  // Check 3x3 subgrids
  for (i = 0; i < 9; i += 3) {
    for (j = 0; j < 9; j += 3) {
      for (k = 0; k <= 9; k++) nums[k] = 0;
      for (k = 0; k < 3; k++) {
        for (l = 0; l < 3; l++) {
          int num = n->sudo[i + k][j + l];
          if (num != 0) {
            if (nums[num] == 1) return 0;
            nums[num] = 1;
          }
        }
      }
    }
  }

  return 1;
}

List* get_adj_nodes(Node* n) {
  List* list = createList();
  int i, j, num;

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) {
        for (num = 1; num <= 9; num++) {
          Node* new_node = copy(n);
          new_node->sudo[i][j] = num;
          if (is_valid(new_node)) {
            pushBack(list, new_node);
          } else {
            free(new_node);
          }
        }
        return list;
      }
    }
  }

  return list;
}

int is_final(Node* n) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) return 0;
    }
  }
  return 1;
}

Node* DFS(Node* initial, int* cont) {
  Stack* stack = createStack();
  push(stack, initial);
  while (!isEmpty(stack)) {
    Node* node = top(stack);
    pop(stack);
    (*cont)++;
    if (is_final(node)) {
      return node;
    }
    List* adj = get_adj_nodes(node);
    Node* adj_node = first(adj);
    while (adj_node != NULL) {
      push(stack, adj_node);
      adj_node = next(adj);
    }
    destroyList(adj);
    free(node);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s <sudoku_file>\n", argv[0]);
    return 1;
  }

  Node* initial = read_file(argv[1]);
  int cont = 0;
  Node* solution = DFS(initial, &cont);

  if (solution) {
    printf("Solution found in %d steps:\n", cont);
    print_node(solution);
    free(solution);
  } else {
    printf("No solution found.\n");
  }

  free(initial);
  return 0;
}


/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/
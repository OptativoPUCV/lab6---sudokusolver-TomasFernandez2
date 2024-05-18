#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int row[9], col[9], subgrid[9];

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            row[j] = col[j] = subgrid[j] = 0;
        }
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] != 0) {
                if(row[n->sudo[i][j] - 1] == 1)
                    return 0;
                row[n->sudo[i][j] - 1] = 1;
            }
            if(n->sudo[j][i] != 0) {
                if(col[n->sudo[j][i] - 1] == 1)
                    return 0;
                col[n->sudo[j][i] - 1] = 1;
            }
        }
        int subgridRow = (i / 3) * 3;
        int subgridCol = (i % 3) * 3;
        for(int r = subgridRow; r < subgridRow + 3; r++){
            for(int c = subgridCol; c < subgridCol + 3; c++){
                if(n->sudo[r][c] != 0) {
                    if(subgrid[n->sudo[r][c] - 1] == 1)
                        return 0;
                    subgrid[n->sudo[r][c] - 1] = 1;
                }
            }
        }
    }
    return 1;
}


List* get_adj_nodes(Node* n){
    List* list = createList();
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0){
                for(int k = 1; k <= 9; k++){
                    Node* new_node = copy(n);
                    new_node->sudo[i][j] = k;
                    if(is_valid(new_node)){
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



int is_final(Node* n){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0)
                return 0;
        }
    }
    return is_valid(n);
}


Node* DFS(Node* initial, int* cont){
    Stack* stack = createStack();
    push(stack, initial);
    while(!isEmpty(stack)){
        Node* node = top(stack);
        pop(stack);
        if(is_final(node)){
            return node;
        }
        List* adj = get_adj_nodes(node);
        for(Node* adjNode = first(adj); adjNode != NULL; adjNode = next(adj)){
            push(stack, adjNode);
        }
        free(node);
        freeList(adj);
        (*cont)++;
    }
    return NULL;
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
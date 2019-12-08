#include "Functions.h"

/*funções relacionadas com criação e manipulação de grafos*/

/*******************************************************************
 *create_grafinho()
 *
 *função que vai criar o grafo, alocando memória para tal;
*******************************************************************/
Graph *create_grafinho(int L, int C, int **matrix) {
  int n_vertex; //nª de vértices
  int i, j, k, p1 = 0, p2 = 0; //(i, j ,k)variáveis de ciclo; (p1, p2)variáveis que vão 
  int index = 0; //indíce de um ponto da cidade
  Graph *grafinho;

  n_vertex = L*C;
  grafinho = init_graph(n_vertex);

  for(i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      for(k = 0; k < 8; k++) {
        point_switcher(&p1, &p2, k);
        if((i + p1 >= 0) && (j + p2 >= 0) && (i + p1 < L) && (j + p2 < C) && matrix[i+p1][j+p2] > 0) {
          fill_lista_adj(grafinho, C, i, j, p1, p2, index, matrix);
        }
      }
      index++;
    }
  }

  return grafinho;
}

Graph *init_graph(int n_vertex) {
  Graph *grafinho;

  grafinho = (Graph *)malloc(sizeof(struct graph));
  if(grafinho == NULL) {
    exit(0);
  }

  grafinho->vertices = n_vertex;
  grafinho->arestas = 0;
  grafinho->list_adj = init_list_adj(n_vertex);

  return grafinho;
}

List **init_list_adj(int n_vertex) {
  int i;
  List **list_adj;

  list_adj = (List **)malloc(n_vertex*sizeof(List *));
  if(list_adj == NULL) {
    exit(0);
  }

  for(i = 0; i < n_vertex; i++) {
    list_adj[i] = initList();
  }

  return list_adj;
}

List *initList (void) {
  return NULL;
}

void fill_lista_adj(Graph *grafinho, int C, int i, int j, int p1, int p2, int id, int **matrix) {
  List *vertex;

  vertex = (List *)malloc(sizeof(List));
  if(vertex == NULL) {
    exit(0);
  }

  vertex->linha = i+p1;
  vertex->coluna = j+p2;
  vertex->peso = matrix[i+p1][j+p2];
  vertex->index = calculate_idx_by_coord(i+p1, j+p2, C);

  grafinho->list_adj[id] = insertUnsortedList(grafinho->list_adj[id], vertex);
}

List *insertUnsortedList(List *next, List *item) {
  List *new;

  /* Memory allocation                                            */
  //new = (List*)malloc(sizeof(LinkedList));

  /* Check memory allocation errors                               */
  //if(new == NULL)
    //return NULL;

  /* Initialize new node                                          */
  new = item;
  new->next = next;

  return new;
}

void freeList(List * first) {
  List * next;
  List * aux;

  for(aux = first; aux != NULL; aux = next) {
    next = aux->next;

    free(aux);
  }

  return;
}

List *getNextNodeList(List *node) {
  return ((node == NULL) ? NULL : node->next);
}

List *getItemList(List *node) {
  if(node == NULL)
    return NULL;

  return node;
}

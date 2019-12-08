#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node List;

struct Node {
  int index;
  int coluna;
  int linha;
  int peso;
  List *next;
};

typedef struct graph {
  int vertices;
  int arestas;
  List **list_adj;
} Graph;

/*********** Testing.c ************/
int check_arg(int argc, char const *argv[], char **nome);
void init_walk_vectors(Graph *grafinho, int **previous_point, int **walk_distance);
int pathing_start(FILE *fp, int *L, int *C, char *path_mode, int *must_visit, int ***matrix, int **point_l, int **point_c, int *invalid);
int calculate_idx_by_coord(int x, int y, int C);
void calculate_coord_by_idx(int *x, int *y, int idx, int C);
void print_solution(FILE *of, int **matrix, int **path, int idx_final, int L, int C, int path_mode, int must_visit, int cost, int steps);
void free_problem(Graph *grafinho, int **matrix, int L, int *point_l, int *point_c, int *previous_point, int *walk_distance, int **path, int cost, int must_visit);


/*********** Pather.c ************/
int walk_mode(Graph *grafinho, int L, int C, char path_mode, int must_visit, int **matrix, int *point_l, int *point_c, int *previous_point, int *walk_distance, int ***path, int *idx_init, int *idx_final, int *steps);
void point_switcher(int *p1, int *p2, int i);
void check_possible(int **matrix, int *point_l, int *point_c, int L, int  C, int i, int *not_move);
void print_walk(int *previous_point, int idx_init, int idx_final);


/*********** Graph.c ************/
/*funções relacionadas com criação e manipulação de grafos*/
Graph *create_grafinho(int L, int C, int **matrix);
Graph *init_graph(int n_vertex);
List **init_list_adj(int n_vertex);
List *initList (void);
void fill_lista_adj(Graph *grafinho, int C, int i, int j, int p1, int p2, int id, int **matrix);
List *insertUnsortedList(List *next, List *item);
void freeList(List * first);
List *getNextNodeList(List *node);
List *getItemList(List *node);


/*********** Short.c ************/
/*funcoes relacionadas com o algoritmo de procura*/
void find_shortest_path(Graph *grafinho, int idx_init, int *previous_point, int *walk_distance, int idx_final);
int *create_queue(Graph *grafinho, int *free, int **tracker);
int next_element_queue(int *queue, int *free, int *walk_distance, int *tracker);
void add_element_queue(Graph *grafinho, int *queue, int *free, int vertice, int *walk_distance, int *tracker);
void update_queue_order(int Idx, int *queue, int *walk_distance, int *tracker);
int check_queue_empty(int free);
void FixUp(int *queue, int Idx, int *walk_distance, int *tracker);
void FixDown(int Idx, int *queue, int *walk_distance, int free, int *tracker);
int biggest_priority(int *walk_distance, int Idx_father, int Idx_insert);
void exchange_queue_values(int *queue, int Idx_replaced, int Idx_switched, int *walk_distance, int *tracker) ;
void free_both(int *queue, int *tracker);

/*funcoes relacionadas com o algoritmo de permutacao*/
void do_permutations_path(int *point_l, int *point_c, int must_visit, Graph *grafinho, int *previous_point, int *walk_distance, int C, int *idx_init, int *idx_final, int *truest_combination_l, int *truest_combination_c, int *minimal_cost, int **speed_matrix);
void recursive_permutation(int *point_l, int *point_c, int new_permute, int must_visit, Graph *grafinho, int *previous_point, int *walk_distance, int C, int *idx_init, int *idx_final, int *truest_combination_l, int *truest_combination_c, int *minimal_cost, int **speed_matrix, int *pos_sp);
void change_points_order(int *point_l, int *point_c, int point1, int point2);
int find_sp_spot(int idx, int must_visit, int *pos_sp);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Functions.h"

int main(int argc, char const *argv[]) {
  int init_res, L, C, must_visit, cost = 0;  //variaveis que guardam os dados de entrada de cada problema;variavel que indica se o problema e possivel ou nao
  int invalid = 0;//indica se foi possivel encontrar um ponto para se mexer
  char path_mode, *nome;  //variaveis que guardam variaveis de entrada de cada problema;
  int **matrix, *point_l, *point_c; //matriz que ira guardar o problema para analise;vetores que guardam os pontos pedidos para analise:l(linha)e c(coluna)
  FILE *fp, *of;  //ponteiros para o ficheiro de abertura e de saida
  Graph *grafinho;
  int *previous_point, *walk_distance, **path;
  int idx_init, idx_final, steps = 0;

  init_res = check_arg(argc, argv, &nome);  //verifica se o nome do ficheiro e o numero de argumentos e aceitavel e guarda o nome do ficheiro de saida numa string (nome)
  if (init_res == 1) { //verificação do erro de argumentos
    return 0;
  }

  fp = fopen(argv[1], "r"); //abrir o ficheiro a ser analisado
  if (fp == NULL) {
    free(nome); //liberta a memória alocada para a string do ficherio de saída
    return 0;
  }

  of = fopen(nome, "w");  //abre o ficheiro de saida

  while (pathing_start(fp, &L, &C, &path_mode, &must_visit, &matrix, &point_l, &point_c, &invalid) != 1) { //funcao que inicializa os dados do problema
    if (invalid != 1) {
     grafinho = create_grafinho(L, C, matrix);
     init_walk_vectors(grafinho, &previous_point, &walk_distance);

     cost = walk_mode(grafinho, L, C, path_mode, must_visit, matrix, point_l, point_c, previous_point, walk_distance, &path, &idx_init, &idx_final, &steps);

     print_solution(of, matrix, path, idx_final, L, C, path_mode, must_visit, cost, steps);

     free_problem(grafinho, matrix, L, point_l, point_c, previous_point, walk_distance, path, cost, must_visit);
     matrix = (int**) NULL;
     point_l = (int*) NULL;
     point_c = (int*) NULL;
     previous_point = (int *) NULL;
     walk_distance = (int *) NULL;
     path = (int **) NULL;
   } else {
     cost = -1;
     fprintf(of, "%d %d %c %d %d %d\n\n", L, C, path_mode, must_visit, cost, 0);
     invalid = 0;
   }
   steps = 0;
   cost = 0;
  }

  fclose(of); //fecha o ficheiro de output com os resultados
  free(nome); //liberta a memória alocada para a string do ficherio de saída

  fclose(fp); //fecha o ficheiro apos ser analisado

  return 0;
}

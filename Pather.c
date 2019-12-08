#include "Functions.h"

/*******************************************************************
 *walk_mode()
 *
 *função que vai, para cada modo de funcionamento, calcular o caminho
 mais curto;
*******************************************************************/
int walk_mode(Graph *grafinho, int L, int C, char path_mode, int must_visit, int **matrix, int *point_l, int *point_c, int *previous_point, int *walk_distance, int ***path, int *idx_init, int *idx_final, int *steps) {
  int final_cost_B = 0, not_move = 0; //custo total na variante B; variável que diz se o problema é possivel ou não
  int i, j, previous, stops = 0; //(i, j) variáveis de ciclo; (previous) variável que vai saber a partir de vértice chega a um dado vértice (iterando sobre previous_point(st) que pela sua natureza já diz isso)
                                 //(stops) em quantos vértices passou num caminho
  int *truest_combination_l, *truest_combination_c, minimal_cost = 100000;
  int **speed_matrix;

  (*path) = (int **)calloc((must_visit - 1), sizeof(int*)); //alocação de um vetor com ponteiros para outros vetores
  if ((*path) == NULL) {                                    //este vetor vai ter tamanho igual ao número de caminhos que irá existir
    exit(0);
  }

  if(path_mode == 'A') {
    if (point_l[0] == point_l[1] && point_c[0] == point_c[1]) { //se o ponto inicial e final são iguais
      return 0;
    }

    for ( i = 0; i < must_visit; i++) { //verifica a possibilidade de todos os pontos pedidos
      check_possible(matrix, point_l, point_c, L, C, i, &not_move); //se algum for impossivel not_move = 1, caso contrário not_move = 0
    }

    if (not_move != 1) { //se todos os pontos forem possíveis
      *idx_init = calculate_idx_by_coord(point_l[0], point_c[0], C); //cálculo do index do ponto inicial
      *idx_final = calculate_idx_by_coord(point_l[1], point_c[1], C); //cálculo do index do ponto final
      find_shortest_path(grafinho, *idx_init, previous_point, walk_distance, *idx_final); //encontrar o caminho mais curto

      if ((previous = previous_point[*idx_final]) == -1) { //se não encontrar um caminho para o ponto destino
        return -1; //cost = -1 --> problema impossível
      }

      //a variável previous vai iterar sobre o previous_point até à origem
      while (previous != -1) { //enquanto não chegar ao destino (pela natureza do previous_point a origem é -1, porque o previous_point diz a partir de que vértice chegou a um dado vértice
        previous = previous_point[previous];
        stops++; //incrementação do nº de vértices que passou
      }

      (*path)[0] = (int *)calloc(stops+1, sizeof(int)); //como aqui só há um caminho pois só há 2 pontos é na 1ª posição
      if ((*path)[0] == NULL) {                         //alocamos um vetor com o tamanho de nº de pontos que passa + 1 (+1 porque uma das posições guarda o stops)
        exit(0);
      }

      (*path)[0][0] = stops; //a primeira posição guarda quantos pontos passou

      previous = previous_point[*idx_final]; //previous = vertice de onde se chegou ao destino

      //este ciclo irá guardar os pontos que passou por ordem inversa (destino - origem), isto pela própria natureza do previous_point(st)
      for (j = 1; j < stops + 1; j++) { //ciclo que começa em 1 pq já foi inserido algo na 1ª posição
        (*path)[0][j] = previous; //o vertice em que passou
        previous = previous_point[previous];
      }

      (*steps) += stops; //adiciona o número de passos total que faz neste problema

      stops = 0; //reset do nº de passos num caminho

      return walk_distance[*idx_final]; //cost = custo total (SPT) para chegar ao vértice destino
    } else
      return -1;
  }

  if(path_mode == 'B') {
    for ( i = 0; i < must_visit; i++) {
      check_possible(matrix, point_l, point_c, L, C, i, &not_move);
    }

    if (not_move != 1) {
      for (i = 0; i < must_visit - 1; i++) { //tem de fazer o caminho entre todos os pontos pedidos (ex: 0-1; 1-2; 2-3; ...); como há n pontos existem n-1 caminhos
        *idx_init = calculate_idx_by_coord(point_l[i], point_c[i], C); //cálculo do index do ponto inicial
        *idx_final = calculate_idx_by_coord(point_l[i+1], point_c[i+1], C); //cálculo do index do ponto final

        if(*idx_init != *idx_final) {
          find_shortest_path(grafinho, *idx_init, previous_point, walk_distance, *idx_final);

          previous = previous_point[*idx_final];

          while (previous != -1) {
            previous = previous_point[previous];
            stops++;
          }

          (*path)[i] = (int *)calloc(stops+1, sizeof(int));
          if ((*path)[i] == NULL) { //se a alocação falhar
            exit(0);
          }

          (*path)[i][0] = stops;

          previous = previous_point[*idx_final];

          for (j = 1; j < stops + 1; j++) {
            (*path)[i][j] = previous;
            previous = previous_point[previous];
          }

          (*steps) += stops;

          final_cost_B += walk_distance[*idx_final];

          stops = 0;
        } else {
          (*path)[i] = (int *)calloc(1, sizeof(int));
          if ((*path)[i] == NULL) { //se a alocação falhar
            exit(0);
          }
          (*path)[i][0]= 0;
        }
      }
      return final_cost_B; //cost = custo total de todos os caminhos
    } else
      return -1;
  }

  if(path_mode == 'C') {
    for ( i = 0; i < must_visit; i++) {
      check_possible(matrix, point_l, point_c, L, C, i, &not_move);
    }

    if(not_move != 1) {
      truest_combination_l = (int *)malloc( must_visit*sizeof(int) );
      if(truest_combination_l == NULL)
        exit(0);

      truest_combination_c = (int *)malloc( must_visit*sizeof(int) );
      if(truest_combination_c == NULL)
        exit(0);

      speed_matrix = (int **)malloc(must_visit*sizeof(int*));
      if(speed_matrix==NULL)
        exit(0);

      for(i = 0; i < must_visit; i++) {
        speed_matrix[i] = (int *)calloc(must_visit, sizeof(int));
        if(speed_matrix[i]==NULL)
          exit(0);
      }

      truest_combination_l[0] = -1;

      do_permutations_path(point_l, point_c, must_visit, grafinho, previous_point, walk_distance, C, &(*idx_init), &(*idx_final), truest_combination_l, truest_combination_c, &minimal_cost, speed_matrix);

      if(truest_combination_l[0] == -1) {
        for(i = 0; i < must_visit; i++) {
          free(speed_matrix[i]);
        }
        free(speed_matrix);
        free(truest_combination_l);
        free(truest_combination_c);
        return -1;
      }

      for (i = 0; i < must_visit - 1; i++) {
        *idx_init = calculate_idx_by_coord(truest_combination_l[i], truest_combination_c[i], C);
        *idx_final = calculate_idx_by_coord(truest_combination_l[i+1], truest_combination_c[i+1], C);

        if(*idx_init != *idx_final) {
          find_shortest_path(grafinho, *idx_init, previous_point, walk_distance, *idx_final);

          previous = previous_point[*idx_final];

          while (previous != -1) {
            previous = previous_point[previous];
            stops++;
          }

          (*path)[i] = (int *)calloc(stops+1, sizeof(int));
          if ((*path)[i] == NULL) { //se a alocação falhar
            exit(0);
          }

          (*path)[i][0] = stops;

          previous = previous_point[*idx_final];

          for (j = 1; j < stops + 1; j++) {
            (*path)[i][j] = previous;
            previous = previous_point[previous];
          }

          (*steps) += stops;

          stops = 0;
        } else {
          (*path)[i] = (int *)calloc(1, sizeof(int));
          if ((*path)[i] == NULL) { //se a alocação falhar
            exit(0);
          }
          (*path)[i][0]= 0;
        }

      }

      for(i = 0; i < must_visit; i++) {
        free(speed_matrix[i]);
      }
      free(speed_matrix);
      free(truest_combination_l);
      free(truest_combination_c);

      return minimal_cost;
    }

    return -1;
  }
  //else
  //  return -1;  caso o problema nao esteja bem definido
}


/*******************************************************************
 *point_switcher()
 *
 *função que irá ver todos os pontos (são 8) à volta que são
 adjacentes a um dado ponto;
*******************************************************************/
void point_switcher(int *p1, int *p2, int i) {
  switch (i) { //vai rodando os pontos necessarios analisar para se mexer em "cavalo"
    case 0:
      *p1 = -1;
      *p2 = 2;
      break;
    case 1:
      *p1 = 1;
      *p2 = 2;
      break;
    case 2:
      *p1 = -2;
      *p2 = 1;
      break;
    case 3:
      *p1 = 2;
      *p2 = 1;
      break;
    case 4:
      *p1 = -2;
      *p2 = -1;
      break;
    case 5:
      *p1 = 2;
      *p2 = -1;
      break;
    case 6:
      *p1 = -1;
      *p2 = -2;
      break;
    case 7:
      *p1 = 1;
      *p2 = -2;
      break;
  }
}

void print_walk(int *previous_point, int idx_init, int idx_final) {
  int start, previous, helper;

  start = idx_init;
  previous = previous_point[idx_final];
  while (previous != start) {
    helper = previous;
    previous = previous_point[previous];
    if (previous == start) {
      printf("%d->", previous);
      start = helper;
      previous = previous_point[idx_final];
      if (previous == start) {
        printf("%d->", previous);
      }
    }
  }
  printf("%d\n", idx_final);
}


/*******************************************************************
 *check_possible()
 *
 *função que verifica se cada ponto pedido pelo problema é válido;
 *ser válido significa se está dentro da matriz (cidade) e se o seu
 valor é diferente de 0; se algum dos pontos não for válido sabemos
 automaticamente que o problema será im+ossível e metemos a variável
 not_move a 1;
*******************************************************************/
void check_possible(int **matrix, int *point_l, int *point_c, int L, int C, int i, int *not_move) {

  if (point_l[i] + 1 > L || point_c[i] + 1 > C || point_l[i] < 0 || point_c[i] < 0 ) { //caso o ponto pedido nao esteja dentro da matriz
    *not_move = 1; //ao meter esta variável a 1 dizemos que o problema é ímpossível e não irá se dar ao trablaho de procurar um caminho
  }

  if (matrix[point_l[i]][point_c[i]] == 0) {  //caso algum ponto pedido seja impossivel
    *not_move = 1;
  }
}

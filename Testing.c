#include "Functions.h"

/*******************************************************************
 *check_arg()
 *
 *função que verifica se os argumentos passados estão corretos em
 número e se o ficheiro passado tem extensão correta (.cities);
 *
 *também obtém a string (a partir de concatenaçao) para abrir o
 ficheiro de saída;
******************************************************************/
int check_arg(int argc, char const *argv[], char **nome) {
  int error = 0;
  char word[] = ".cities";

  if (argc != 2) { //verifica o número de argumentos
    error = 1;
    return error;
  }

  if (strstr(argv[1], word) == NULL) { //verifica a extensão do documento
    error = 1;
    return error;
  } else {
    *nome = (char *)malloc((strlen(argv[1]) - strlen(".cities")+strlen(".walks") + 1)*sizeof(char)); //alocação da string do ficheiro de saída com o tamanho apropriado
    if(*nome == NULL)
      exit(0);
    strncpy(*nome, argv[1], strlen(argv[1]) - strlen(".cities")); //copia o nome do ficheiro de entrada até ".cities" para a string que guardara o nome do ficheiro de saída
    (*nome)[strlen(argv[1]) - strlen(".cities")]='\0'; //mete a última posição da string do ficheiro de saída a \0 para poder fazer concatenaçao
    strcat(*nome, ".walks"); //concatena o nome com .walks
    (*nome)[strlen(*nome)]='\0'; //mete o último elemento a \0
  }

  return error;
}


/*******************************************************************
 *init_walk_vectors()
 *
 *função que inicializa (aloca memória para) os vetores que guardarão
 caminho mínimo de um dado ponto a outro e o peso que custa a ir para
 dado ponto;
*******************************************************************/
void init_walk_vectors(Graph *grafinho, int **previous_point, int **walk_distance) {
  int i;

  //aloca memória para os vetores cada um com o tamanho igual ao número
  //de vertices (pontos) do problema
  *previous_point = (int*)malloc( (grafinho->vertices)*sizeof(int) );
  if(*previous_point == NULL)
    exit(0);

  *walk_distance = (int*)malloc( (grafinho->vertices)*sizeof(int) );
  if(*walk_distance == NULL)
    exit(0);

  return;
}


/*******************************************************************
 *pathing_start()
 *
 *função que lê o problema e guarda os dados;
 *guarda os pontos em que tem que passar em dois vetores distintos
 (um para a coordena x e outro para coordenada y); guarda cidade
 numa matriz; também guarda o modo de funcionamento (A, B ou C) em
 path_mode, a quantidade de pontos que tem que passar (must_visit),
 as linhas e as colunas;
*******************************************************************/
int pathing_start(FILE *fp, int *L, int *C, char *path_mode, int *must_visit, int ***matrix, int **point_l, int **point_c, int *invalid) {
  int i, j, end = 0, test_value = 0;

  //fscanf retorno a quantidade de variáveis que leu, portanto se não conseguir ler as 4 variáveis necessárias sabe-se que chegou ao fim do
  //ficheiro e mete uma variável (end) para se saber tal
  if (fscanf(fp, "%d %d %c %d", &(*L), &(*C) , &(*path_mode), &(*must_visit) ) != 4) { //analisar a primeira linha para guardar as linhas,
    end = 1;                                                                           //as colunas, o modo de funcionamento e a quantidade
    return(end);                                                                       //de pontos que terá que passar
  }

  //só se alocará memória para a matriz e os vetores que guardam os pontos que se querem visitar se o modo de funcionament for um dos
  //permitidos (A, B ou C) e se must_visit for o correto para cada modo
  if ((*path_mode == 'A' && *must_visit == 2) || (*path_mode == 'B' && *must_visit >= 2) || (*path_mode == 'C' && *must_visit >= 2)) {
    (*point_l) = (int *)malloc((*must_visit)*sizeof(int)); //alocar memoria para um vetor que guarda a coordenada y(L) dos pontos a visitar
    if((*point_l) == NULL) { //se a alocação falhar
      exit(0);
    }

    (*point_c) = (int *)malloc((*must_visit)*sizeof(int));//alocar memoria para um vetor que guarda a coordenada x(C) dos pontos a visitar
    if((*point_c) == NULL) { //se a alocação falhar
      exit(0);
    }
    for(i = 0; i < *must_visit; i++) {  //guardar todos os pontos que sao obrigatorios de serem visitados
      test_value = fscanf(fp, "%d %d", &(*point_l)[i], &(*point_c)[i]); //guardar os pontos
    }

    (*matrix) = (int **)malloc((*L)*sizeof(int*));
    if (*matrix == NULL) { //se a alocação falhar
      exit(0);
    }

    for (i = 0; i < *L; i++) { //cada um desses ponteiros desse vetor irá apontar para um vetor com o tamanho das colunas da matriz
      (*matrix)[i] = (int *)malloc((*C)*sizeof(int));
      if ((*matrix)[i] == NULL) { //se a alocação falhar
        exit(0);
      }
    }

    for (i = 0; i < *L; i++) {  //enche a matrix com os elementos presentes no ficheiro
      for (j = 0; j < *C; j++) {
        test_value = fscanf(fp, "%d", &(*matrix)[i][j]);
      }
    }
  } else { //se nenhuma das condições se verificar o problema está mal condicionado; tem que se saltar o problema e passar para o próximo
    //salta os pontos
    for(i = 0; i < *must_visit; i++) {
      test_value = fscanf(fp, "%d %d", &test_value, &test_value);
    }

    //salta a cidade
    for (i = 0; i < *L; i++) {
      for (j = 0; j < *C; j++) {
        test_value = fscanf(fp, "%d", &test_value);
      }
    }
    *invalid = 1; //esta variável irá dizer se o problema é inválido e na main imprimimos corretamente
  }

  return end;
}


/*******************************************************************
 *calculate_idx_by_coord()
 *
 *calcula o número do vertice da cidade a partir das suas coordenadas;
*******************************************************************/
int calculate_idx_by_coord(int x, int y, int C) {
  int idx;

  idx = (x*C) + y;

  return idx;
}


/*******************************************************************
 *calculate_coord_by_idx()
 *
 *calcula as coordenadas de um dado vértice (a partir do seu
 índice/número) tem na cidade;
*******************************************************************/
void calculate_coord_by_idx(int *x, int *y, int idx, int C) {
  *x = idx%C;
  *y = idx/C;
}


/*******************************************************************
 *print_solution()
 *
 *função que imprime no ficheiro de saída a solução (custo, nª de
 passos e todos os pontos que passou);
*******************************************************************/
void print_solution(FILE *of, int **matrix, int **path, int idx_final, int L, int C, int path_mode, int must_visit, int cost, int steps) {
  int x, y, first_time = 1; //(x,y)variáveis que dirão as coordenadas de um dado vértice na cidade; variável que indica se é a primeira vez que imrpime e não imprime o ponto de partida
  int i, j; //variáveis de ciclo

  if (cost == -1 || cost == 0) { //se o problema for não tiver solução ou se custa zero
    steps = 0;
    fprintf(of, "%d %d %c %d %d %d\n\n", L, C, path_mode, must_visit, cost, steps); //imprime no ficheiro os resultados dos teste
  } else {
    fprintf(of, "%d %d %c %d %d %d\n", L, C, path_mode, must_visit, cost, steps); //imprime no ficheiro os resultados dos teste

    //imprimir os passos
    for (i = 0; i < must_visit - 1; i++) { //itera sobre o vetor que guarda os caminhos feitos (vetor de ponteiros, cada ponteiro aponta para um vetor)
      //tem que se percorrer a partir do ínicio porque a ordem que se guardou o caminho foi na ordem inversa (último ponto - ponto inicial) de cada caminho
      //isto pela natureza do vetor previous_point (st nos acetatos) que está no algoritmo Dijkstra

      if(path[i][0] != 0) {
        for (j = path[i][0]; j > 0; j--) { //itera sobre cada vetor (este vetor guarda os vértices que fez para cada caminho) que está em cada
                                           //posição do vetor de ponteiros, mas tem de fazer isto sem fazer a 1ª posição que guarda a quantidade de pontos
          if (first_time == 1) { //se é o primeiro ponto nao deve imprimir
            first_time = 0;
          } else {
            calculate_coord_by_idx(&x, &y, path[i][j], C); //calcula as coordenadas do vértice pelo se índice
            fprintf(of, "%d %d %d\n", y, x, matrix[y][x]); //imprime o ponto
          }
        }
      }
    }
    //imprimir o último ponto do caminho; faz-se isso porque da maneira que se guardou o caminho, não está lá guardado o último ponto
    calculate_coord_by_idx(&x, &y, idx_final, C); //calcula as coordenadas do vértice pelo se índice
    fprintf(of, "%d %d %d\n\n", y, x, matrix[y][x]); //imprime o ultimo ponto (destino) do caminho total
  }
}


/*******************************************************************
 *free_problem()
 *
 *função que libertará cada vetor ou matriz para que foi alocado
 memória dinamicamente; também liberta a memória alocada para a
 estrutura do grafo, incluido o vetor que guarda lista e cada lista;
*******************************************************************/
void free_problem(Graph *grafinho, int **matrix, int L, int *point_l, int *point_c, int *previous_point, int *walk_distance, int **path, int cost, int must_visit) {
  int i;  //variavel de ciclo

  free(previous_point);
  free(walk_distance);

  for (i = 0; i < grafinho->vertices; i++) {
    freeList(grafinho->list_adj[i]); //libertação de cada lista
  }
  free(grafinho->list_adj); //libertação do vetor que guarda a lista
  free(grafinho); //libertação da estrutura do grafo

  for (i = 0; i < L; i++) {
    free(matrix[i]);  //desaloca cada linha da matriz
  }
  free(matrix); //liberta o resto da matriz


  if (cost != -1 || cost != 0) { //só se o problema é possível porque so fazemos alocação de memória para os vetores que estão em cada posição do vetor de ponteiros
                                //quando o problema é possível
    for (i = 0; i < must_visit - 1; i++) {
      free(path[i]); //libertar cada vetor (que guarda os pontos que passou em dado caminho)
    }
  }
  free(path); //liberta o vetor de ponteiros (os caminhos todos feitos)

  free(point_l);
  free(point_c);
}

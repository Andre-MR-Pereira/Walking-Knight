#include "Functions.h"

void find_shortest_path(Graph *grafinho, int idx_init, int *previous_point, int *walk_distance, int idx_final) {
  int vertice;
  int free, *queue, *tracker;
  int max_walk_distance = 100000, no_point = -1;
  int analysing;
  List *aux;

  queue = create_queue(grafinho, &free, &tracker);

  for(vertice = 0; vertice < grafinho->vertices; vertice++) {
    walk_distance[vertice] = max_walk_distance;
    previous_point[vertice] = no_point;
    add_element_queue(grafinho, queue, &free, vertice, walk_distance, tracker);
  }
  walk_distance[idx_init] = 0;
  update_queue_order(idx_init, queue, walk_distance, tracker);

  while(check_queue_empty(free) != 1) {
    if( walk_distance[analysing = next_element_queue(queue, &free, walk_distance, tracker)] != max_walk_distance) {
      for(aux = grafinho->list_adj[analysing]; aux != NULL; aux = aux->next) {
        if( walk_distance[aux->index] >= (walk_distance[analysing] + aux->peso)) {
          walk_distance[aux->index] = walk_distance[analysing] + aux->peso;
          update_queue_order(aux->index, queue, walk_distance, tracker);
          previous_point[aux->index] = analysing;
        }
        if (previous_point[idx_final] != no_point) {
          free_both(queue, tracker);
          return;
        }
      }
    }
  }

  free_both(queue, tracker);

  return;
}

int *create_queue(Graph *grafinho, int *free, int **tracker) {
  int *queue;

  queue = (int *)malloc( (grafinho->vertices) * sizeof(int) );
  if(queue == NULL) {
    exit(0);
  }

  (*tracker) = (int *)malloc( (grafinho->vertices) * sizeof(int) );
  if( (*tracker) == NULL) {
    exit(0);
  }

  *free = 0;

  return queue;
}

int next_element_queue(int *queue, int *free, int *walk_distance, int *tracker) {
  exchange_queue_values(queue, queue[0], queue[(*free)-1], walk_distance, tracker);
  FixDown(0, queue, walk_distance, (*free)-1, tracker);
  return queue[--(*free)];
}

void add_element_queue(Graph *grafinho, int *queue, int *free, int vertice, int *walk_distance, int *tracker) {

  if( (*free) < (grafinho->vertices) ) {
    queue[(*free)] = vertice;
    tracker[vertice]= (*free);
    FixUp(queue, (*free), walk_distance, tracker);
    (*free)++;
  }

  return;
}

void update_queue_order(int Idx, int *queue, int *walk_distance, int *tracker) {
  FixUp(queue, Idx, walk_distance, tracker);
  return;
}

int check_queue_empty(int free) {
  if(free == 0)
    return 1;

  return 0;
}

void FixUp(int *queue, int Idx, int *walk_distance, int *tracker) {
  int i;

  while ( tracker[Idx] > 0 && (biggest_priority(walk_distance, queue[ ((tracker[Idx])-1)/2], Idx) == 1) ) {
    exchange_queue_values(queue, queue[ ((tracker[Idx])-1)/2], Idx, walk_distance, tracker);
  }

  return;
}

void FixDown(int Idx, int *queue, int *walk_distance, int free, int *tracker) {
  int Child;
  int i;

  while(2*Idx < free-1) {
    Child = 2*Idx +1;
    if(Child < free-1 && biggest_priority(walk_distance, queue[Child], queue[Child+1])  ==  1 )
      Child++;

    if(biggest_priority(walk_distance, queue[Idx], queue[Child]) == 0)
      break;

    exchange_queue_values(queue, queue[Child], queue[Idx], walk_distance, tracker);
    Idx = Child;
  }

  return;
}

int biggest_priority(int *walk_distance, int Idx_father, int Idx_insert) {
  if(walk_distance[Idx_father] > walk_distance[Idx_insert])
    return 1;
  else
    return 0;
}

void exchange_queue_values(int *queue, int Idx_replaced, int Idx_switched, int *walk_distance, int *tracker) {
  int holder;

  holder = queue[tracker[Idx_replaced]];
  queue[tracker[Idx_replaced]] = queue[tracker[Idx_switched]];
  queue[tracker[Idx_switched]] = holder;

  holder = tracker[Idx_replaced];
  tracker[Idx_replaced] = tracker[Idx_switched];
  tracker[Idx_switched] = holder;

  return;
}

void free_both(int *queue, int *tracker) {
  free(queue);
  free(tracker);
}

void do_permutations_path(int *point_l, int *point_c, int must_visit, Graph *grafinho, int *previous_point, int *walk_distance, int C, int *idx_init, int *idx_final, int *truest_combination_l, int *truest_combination_c, int *minimal_cost, int **speed_matrix) {
  int i, j, *pos_sp;

  pos_sp=(int*)malloc(must_visit*sizeof(int));

  for(i=0;i<must_visit;i++) {
    pos_sp[i]=calculate_idx_by_coord(point_l[i],point_c[i],C);
  }

  recursive_permutation(point_l, point_c, 1, must_visit, grafinho, previous_point, walk_distance, C, &(*idx_init), &(*idx_final), truest_combination_l, truest_combination_c, &(*minimal_cost), speed_matrix, pos_sp);

  free(pos_sp);
}

void recursive_permutation(int *point_l, int *point_c, int new_permute, int must_visit, Graph *grafinho, int *previous_point, int *walk_distance, int C, int *idx_init, int *idx_final, int *truest_combination_l, int *truest_combination_c, int *minimal_cost, int **speed_matrix, int *pos_sp) {
  int i,j,bigger_path=0,permutation_path=0;

  if(new_permute == must_visit) {
    for(i = 0; i < must_visit-1; i++) {
      *idx_init = calculate_idx_by_coord(point_l[i], point_c[i], C);
      *idx_final = calculate_idx_by_coord(point_l[i+1], point_c[i+1], C);

      if( (*idx_init!=*idx_final) && speed_matrix[find_sp_spot(*idx_init, must_visit, pos_sp)][find_sp_spot(*idx_final, must_visit, pos_sp)] != 0) {
        permutation_path += speed_matrix[find_sp_spot(*idx_init, must_visit, pos_sp)][find_sp_spot(*idx_final, must_visit, pos_sp)];
        if(permutation_path > (*minimal_cost))
          bigger_path = 1;
      } else if(*idx_init!=*idx_final) {
        find_shortest_path(grafinho, *idx_init, previous_point, walk_distance, *idx_final);
        speed_matrix[find_sp_spot(*idx_init,must_visit,pos_sp)][find_sp_spot(*idx_final,must_visit,pos_sp)] = walk_distance[*idx_final];

        permutation_path += walk_distance[*idx_final];

        if(permutation_path > (*minimal_cost) )
          bigger_path = 1;
      }

      if(bigger_path == 1)
        break;
    }

    if(bigger_path == 0) {
      for(j = 0; j < must_visit; j++) {
        truest_combination_l[j] = point_l[j];
        truest_combination_c[j] = point_c[j];
      }
      *minimal_cost = permutation_path;
    }
  } else {
    for(i = new_permute; i < must_visit; i++) {
    change_points_order(point_l,point_c,new_permute,i);
    recursive_permutation(point_l,point_c,new_permute+1,must_visit,grafinho,previous_point,walk_distance,C,&(*idx_init),&(*idx_final),truest_combination_l,truest_combination_c,&(*minimal_cost),speed_matrix,pos_sp);
    change_points_order(point_l,point_c,i,new_permute);
    }
  }
  return;
}

void change_points_order(int *point_l, int *point_c, int point1, int point2) {
  int buffer;

  buffer = point_l[point1];
  point_l[point1] = point_l[point2];
  point_l[point2] = buffer;

  buffer = point_c[point1];
  point_c[point1] = point_c[point2];
  point_c[point2] = buffer;

  return;
}

int find_sp_spot(int idx, int must_visit, int *pos_sp) {
  int i;

  for(i = 0; i < must_visit; i++) {
    if(pos_sp[i] == idx)
      return i;
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "print.h"
#include "selection.h"
#include "fitness.h"
#include "read_data.h"
#include "muta_cross.h"
#include "structs.h"
#include "defines.h"


/*'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/
int main(void){
  /*init general stuff*/
  int i, j; 
  int h_classes[NUMBER_OF_HEAVY_LESSONS] = {mat, fys, eng, dan, tys};
  /* Allocating space for arrays of structs*/
  individual **population;
  population = (individual **)calloc(NUMBER_OF_CLASSES, sizeof(individual *));

  individual **old_population;
  old_population = (individual **)calloc(NUMBER_OF_CLASSES, sizeof(individual *));

  individual **chosen_individual;
  chosen_individual = (individual **)calloc(NUMBER_OF_CLASSES, sizeof(individual *));

  individual *best_of_best;
  best_of_best = (individual *)calloc(NUMBER_OF_CLASSES, sizeof(individual ));

  requirements *requirements_classes;
  requirements_classes = (requirements *)calloc(NUMBER_OF_CLASSES, sizeof(requirements));

  class_info **class_data;
  class_data = (class_info **)calloc(NUMBER_OF_CLASSES, sizeof(class_info *));
  /* Allocating space for double arrays*/
  for(i = 0; i < NUMBER_OF_CLASSES; i++){
    population[i] = (individual *)calloc(SIZE_OF_POPULATION +3, sizeof(individual));
    old_population[i] = (individual *)calloc(SIZE_OF_POPULATION, sizeof(individual));
    chosen_individual[i] = (individual *)calloc(NUMBER_OF_GENERATIONS, sizeof(individual));
    class_data[i] = (class_info *)calloc(NUMBER_OF_SUBJECTS, sizeof(class_info));
  }

  read_teachers_name(class_data);
  find_req(class_data, requirements_classes);

  /* Init genetic algorithem */
  srand(time(NULL));
  create_individuals(population);
  calculate_fitness_all(population, h_classes, class_data, requirements_classes);

  /* Genetic algorithem */
  for(i = 0; i < NUMBER_OF_GENERATIONS; i++){


    /* Selektion */
    selektion(population, old_population);

    /* Mutation */
    mutation(population);

    /* Crossover */
    crossover(population, old_population, requirements_classes);

    /* Fitness */
    calculate_fitness_all(population, h_classes, class_data, requirements_classes);

    /* Tests */
    for(j = 0; j < (NUMBER_OF_CLASSES); j += 3){
      choose_individual(population, chosen_individual, j, i);  
    }
    
    printf("%d   %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \n",i , chosen_individual[0][i].fitness, chosen_individual[0][i].perfection , chosen_individual[1][i].fitness, chosen_individual[1][i].perfection, chosen_individual[2][i].fitness, chosen_individual[2][i].perfection, chosen_individual[3][i].fitness, chosen_individual[3][i].perfection, chosen_individual[4][i].fitness, chosen_individual[4][i].perfection, chosen_individual[5][i].fitness, chosen_individual[5][i].perfection, chosen_individual[6][i].fitness, chosen_individual[6][i].perfection, chosen_individual[7][i].fitness, chosen_individual[7][i].perfection, chosen_individual[8][i].fitness, chosen_individual[8][i].perfection);

  }
   i--;
  /* The best of the best */
  find_best(chosen_individual, best_of_best);
  /* Printing */
  printf("\n\n\n");
  print_func(best_of_best, requirements_classes, class_data);

  /*Freeing alloceted space*/
  for(i = 0; i < NUMBER_OF_CLASSES; i++){
    free(population[i]);
    free(old_population[i]);
    free(class_data[i]);
    free(chosen_individual[i]);
  }
  free(population);
  free(old_population);
  free(chosen_individual);
  free(best_of_best);
  free(class_data);
  free(requirements_classes);

  return 0; 
}
/*'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/


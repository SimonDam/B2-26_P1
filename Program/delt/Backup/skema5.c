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
  /* init general stuff */
  int h_classes[NUMBER_OF_HEAVY_LESSONS] = {mat, fys, eng, dan, tys};

  /* init classes */
  individual individuals[NUMBER_OF_CLASSES][NUMBER_OF_INDIVIDUALS];
  individual chosen_individual[NUMBER_OF_CLASSES][NUMBER_OF_GENERATIONS];
  individual best_of_best[NUMBER_OF_CLASSES];
  individual individuals_temp[NUMBER_OF_CLASSES][NUMBER_OF_INDIVIDUALS];
  requirements requirements_classes[NUMBER_OF_CLASSES];
  
  /*Finds number of teacher and reads in info*/
  teacher teacher_data[NUMBER_OF_CLASSES][NUMBER_OF_SUBJECTS];
  read_teachers_name(teacher_data);
  find_req(teacher_data, requirements_classes);

  /* Init genetic algorithem */
  srand(time(NULL));

  create_individuals(individuals);
  calculate_fitness_all(individuals, h_classes, teacher_data, requirements_classes);

  int i, j;

 for(j = 0; j < NUMBER_OF_CLASSES; j += 3){
    choose_individual(individuals, chosen_individual, j, 0);
  }
 
  /* Genetic algorithem */
  for (i = 0; i < NUMBER_OF_GENERATIONS; i++){
    /* Selektion */
    selektion(individuals);
    
    /* Mutation */
    mutation(individuals);

    /* Crossover */
    crossover(individuals, individuals_temp, requirements_classes, i);

    /* Fitness */
    calculate_fitness_all(individuals, h_classes, teacher_data, requirements_classes);

    /* Tests */
    for(j = 0; j < (NUMBER_OF_CLASSES); j += 3){
      choose_individual(individuals, chosen_individual, j, i);  
    }
    printf("%d   %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \t %d %d \n",i , chosen_individual[0][i].fitness, chosen_individual[0][i].perfection , chosen_individual[1][i].fitness, chosen_individual[1][i].perfection, chosen_individual[2][i].fitness, chosen_individual[2][i].perfection, chosen_individual[3][i].fitness, chosen_individual[3][i].perfection, chosen_individual[4][i].fitness, chosen_individual[4][i].perfection, chosen_individual[5][i].fitness, chosen_individual[5][i].perfection, chosen_individual[6][i].fitness, chosen_individual[6][i].perfection, chosen_individual[7][i].fitness, chosen_individual[7][i].perfection, chosen_individual[8][i].fitness, chosen_individual[8][i].perfection);
  
  }
  i--;
  /* The best of the best */
  find_best(chosen_individual, best_of_best);

  /* Printing */
  printf("\n\n\n");
  print_func(best_of_best, requirements_classes, i, teacher_data);

  return 0;
}
/*'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/


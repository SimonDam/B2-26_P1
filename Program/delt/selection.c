#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "selection.h"
#include "muta_cross.h"

void selektion(individual individuals[][NUMBER_OF_INDIVIDUALS]){
  individual individuals_temp[NUMBER_OF_CLASSES][NUMBER_OF_INDIVIDUALS];
  int i, res;

  /* Makes individuals_temp equal to individuals */
  make_old_population(individuals, individuals_temp);

  /* Runs trough all individuals, 3 classes at a time (parralelclasses) and picks a combination of parralelclasses for each grade (7, 8 and 9)*/
  for(int j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for (i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      res = pick_individual(individuals_temp, individuals, j, i);
    }
  }
}

/* int class represents 7, 8, 9 class as: 0 = 7a, 1 = 7b, 2 = 7c, 3 = 8a, 4 = 8b and so on. lesson_number tells which individual is currently being changed */
int pick_individual(individual temp_individuals[][NUMBER_OF_INDIVIDUALS], individual individuals[][NUMBER_OF_INDIVIDUALS], int class, int lesson_number){
  int i, sum = 0, j;
  int fitness_test = 0;
  int sum_parrallel[NUMBER_OF_INDIVIDUALS];

  /* First the fitness from all individuels is added together to make the roulette */
  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    if ((temp_individuals[class][i].fitness == 1) || (temp_individuals[class+1][i].fitness == 1) || (temp_individuals[class+2][i].fitness == 1)){
      sum_parrallel[i] = 1;
    }
    else {
      sum_parrallel[i] = temp_individuals[class][i].fitness + temp_individuals[class+1][i].fitness + temp_individuals[class+2][i].fitness;
    }
    sum += sum_parrallel[i];
  }

  /* Then a random spot is chosen on the roulette */

  int field = rand()% sum;

  /* Then starts from the bottum and adds the sum of the fitness for three parallelclasses to the sum of the previous fitness (fitness test). Then checks if the 
     summed fitness if greater than field, in which case the random spot is found and the current individuals are being saved*/
  
  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    fitness_test += sum_parrallel[i];
    if(field < fitness_test){
      individuals[class][lesson_number] = temp_individuals[class][i];
      individuals[class+1][lesson_number] = temp_individuals[class+1][i];
      individuals[class+2][lesson_number] = temp_individuals[class+2][i];
      return 1;
    }
  } 
  return 0;
}

/* function finds the three parralelclasses with the highest summed fitness
/* int class represents 7, 8, 9 class as: 0 = 7a, 1 = 7b, 2 = 7c, 3 = 8a, 4 = 8b and so on. generation interger tells which individuals is currently being changed*/

void choose_individual(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_chosen[][NUMBER_OF_GENERATIONS], int class, int generation){
  int choose_fitness = -1;
  int individual_test = -2;
  int sum_parrallel_fitness[NUMBER_OF_INDIVIDUALS];
  int i, j, k, f;
  
  calculate_finess_parallel(individuals, sum_parrallel_fitness, class);

  for(k = 0; k < NUMBER_OF_INDIVIDUALS; k++){
    if (sum_parrallel_fitness[k] > choose_fitness){
      individuals_chosen[class][generation] = individuals[class][k];
      individuals_chosen[class+1][generation] = individuals[class+1][k];
      individuals_chosen[class+2][generation] = individuals[class+2][k];
      choose_fitness = sum_parrallel_fitness[k];
      individual_test = k;
    }
  }
}

/* Adds the fitness of three parallelclasses to eachother, since they are dependent of eachother */
void calculate_finess_parallel(individual individuals[][NUMBER_OF_INDIVIDUALS], int sum_parrallel[], int class){
  int i;
  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    if((individuals[class][i].fitness == 1) || (individuals[class+1][i].fitness == 1) || (individuals[class+2][i].fitness == 1)){
      sum_parrallel[i] = 1;
    }
    else{
    sum_parrallel[i] = individuals[class][i].fitness + individuals[class+1][i].fitness + individuals[class+2][i].fitness;
    }
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "selection.h"
#include "muta_cross.h"

/*Selection of the best individuals in a population using the roulette method */
void selektion(individual **population, individual **old_population){
  int i, j;  
  /* Makes old_population equal to populatoin */
  make_old_population(population, old_population);

  /* Runs trough all population, 3 classes at a time (parralelclasses) and picks a combination of parralelclasses for each grade (7, 8 and 9)*/
  for(j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for (i = 0; i < SIZE_OF_POPULATION; i++){
      pick_individual(population, old_population, j, i);
    }
  }
}

/* int class represents 7, 8, 9 class as: 0 = 7a, 1 = 7b, 2 = 7c, 3 = 8a, 4 = 8b and so on. indi_num tells which individual is currently being changed */
int pick_individual(individual **population, individual **old_population, int class, int indi_num){
  int i, j;
  int fitness_test = 0, sum = 0;
  int *sum_parrallel;
  sum_parrallel = (int *)calloc(SIZE_OF_POPULATION, sizeof(int));


  /* First the fitness from all individuels is added together to make the roulette */
  for(i = 0; i < SIZE_OF_POPULATION; i++){
    if ((old_population[class][i].fitness == 1) || (old_population[class+1][i].fitness == 1) || (old_population[class+2][i].fitness == 1)){
      sum_parrallel[i] = 1;
    }
    else {
      sum_parrallel[i] = old_population[class][i].fitness + old_population[class+1][i].fitness + old_population[class+2][i].fitness;
    }
    sum += sum_parrallel[i];
  }

  /* Then a random spot is chosen on the roulette */

  int field = rand()% sum;

  /* Then starts from the bottum and adds the sum of the fitness for three parallelclasses to the sum of the previous fitness (fitness test). Then checks if the 
     summed fitness if greater than field, in which case the random spot is found and the current population are being saved*/
  
  for(i = 0; i < SIZE_OF_POPULATION; i++){
    fitness_test += sum_parrallel[i];
    if(field < fitness_test){
      population[class][indi_num] = old_population[class][i];
      population[class+1][indi_num] = old_population[class+1][i];
      population[class+2][indi_num] = old_population[class+2][i];
      return 1;
    }
  }
  free(sum_parrallel); 
  return 0;
}

/* function finds the three parralelclasses with the highest summed fitness
int class represents 7, 8, 9 class as: 0 = 7a, 1 = 7b, 2 = 7c, 3 = 8a, 4 = 8b and so on. generation interger tells which population is currently being changed*/

void choose_individual(individual **population, individual **chosen_individual, int class, int generation){
  int i;
  int choose_fitness = -1;
  int individual_test = -2;
  int sum_parrallel_fitness[SIZE_OF_POPULATION];

  calculate_finess_parallel(population, sum_parrallel_fitness, class);

  for(i = 0; i < SIZE_OF_POPULATION; i++){
    if (sum_parrallel_fitness[i] > choose_fitness){
      chosen_individual[class][generation] = population[class][i];
      chosen_individual[class+1][generation] = population[class+1][i];
      chosen_individual[class+2][generation] = population[class+2][i];
      choose_fitness = sum_parrallel_fitness[i];
    }
  }

}

/* Adds the fitness of three parallelclasses to eachother, since they are dependent of eachother */
void calculate_finess_parallel(individual **population, int sum_parrallel[], int class){
  int i;
  for(i = 0; i < SIZE_OF_POPULATION; i++){
    if((population[class][i].fitness == 1) || (population[class+1][i].fitness == 1) || (population[class+2][i].fitness == 1)){
      sum_parrallel[i] = 1;
    }
    else{
    sum_parrallel[i] = population[class][i].fitness + population[class+1][i].fitness + population[class+2][i].fitness;
    }
  }
}

/*Finding best individuals from all generations*/
void find_best(individual **chosen_population, individual best_of_best[]){
  printf("\n\n");
  int i, k, j;
  int best_sum1, best_sum2, best_sum3;
  int best_gen1, best_gen2, best_gen3;
  int sum1, sum2, sum3;
  int temp_perfect1 = 0, temp_perfect2 = 0, temp_perfect3 = 0;
  int perfect1 = 0, perfect2 = 0, perfect3 = 0;
  
  for (i = 0; i < NUMBER_OF_GENERATIONS; i++){
    /* Getting the sum */
    if ((chosen_population[0][i].fitness != 1) && (chosen_population[1][i].fitness != 1) && (chosen_population[2][i].fitness != 1)){
      sum1 = chosen_population[0][i].fitness + chosen_population[1][i].fitness + chosen_population[2][i].fitness;
    }
    else {
      sum1 = 0;
    }

    if ((chosen_population[3][i].fitness != 1) && (chosen_population[4][i].fitness != 1) && (chosen_population[5][i].fitness != 1)){
      sum2 = chosen_population[3][i].fitness + chosen_population[4][i].fitness + chosen_population[5][i].fitness;
    }
    else {
      sum2 = 0;
    }

    if ((chosen_population[6][i].fitness != 1) && (chosen_population[7][i].fitness != 1) && (chosen_population[8][i].fitness != 1)){
      sum3 = chosen_population[6][i].fitness + chosen_population[7][i].fitness + chosen_population[8][i].fitness;
    }
    else {
      sum3 = 0;
    }

    /* Getting the lowest perfection grade */
    temp_perfect1 = 15;
    temp_perfect2 = 15;
    temp_perfect3 = 15;

    if (temp_perfect1 > chosen_population[0][i].perfection){
      temp_perfect1 = chosen_population[0][i].perfection;
    }
    if (temp_perfect1 > chosen_population[1][i].perfection){
      temp_perfect1 = chosen_population[1][i].perfection;
    }
    if (temp_perfect1 > chosen_population[2][i].perfection){
      temp_perfect1 = chosen_population[2][i].perfection;
    }

    if (temp_perfect2 > chosen_population[3][i].perfection){
      temp_perfect2 = chosen_population[3][i].perfection;
    }
    if (temp_perfect2 > chosen_population[4][i].perfection){
      temp_perfect2 = chosen_population[4][i].perfection;
    }
    if (temp_perfect2 > chosen_population[5][i].perfection){
      temp_perfect2 = chosen_population[5][i].perfection;
    }

    if (temp_perfect3 > chosen_population[6][i].perfection){
      temp_perfect3 = chosen_population[6][i].perfection;
    }
    if (temp_perfect3 > chosen_population[7][i].perfection){
      temp_perfect3 = chosen_population[7][i].perfection;
    }
    if (temp_perfect3 > chosen_population[8][i].perfection){
      temp_perfect3 = chosen_population[8][i].perfection;
    }

    /* Saving the best */
    if (perfect1 < temp_perfect1){
      best_sum1 = sum1;
      best_gen1 = i;
      perfect1 = temp_perfect1;
      printf("  1  perfection: %d  sum: %d \n", perfect1, best_sum1);
    }
    else if (perfect1 == temp_perfect1){
      if (best_sum1 < sum1){
        best_sum1 = sum1;
        best_gen1 = i;
        perfect1 = temp_perfect1;
        printf("  1  perfection: %d  sum: %d \n", perfect1, best_sum1);
      }
    }

    if (perfect2 < temp_perfect2){
      best_sum2 = sum2;
      best_gen2 = i;
      perfect2 = temp_perfect2;
      printf("  2  perfection: %d  sum: %d \n", perfect2, best_sum2);
    }
    else if (perfect2 == temp_perfect2){
      if (best_sum2 < sum2){
        best_sum2 = sum2;
        best_gen2 = i;
        perfect2 = temp_perfect2;
        printf("  2  perfection: %d  sum: %d \n", perfect2, best_sum2);
      }
    }

    if (perfect3 < temp_perfect3){
      best_sum3 = sum3;
      best_gen3 = i;
      perfect3 = temp_perfect3;
      printf("  3  perfection: %d  sum: %d \n", perfect3, best_sum3);
    }
    else if (perfect3 == temp_perfect3){
      if (best_sum3 < sum3){
        best_sum3 = sum3;
        best_gen3 = i;
        perfect3 = temp_perfect3;
        printf("  3  perfection: %d  sum: %d \n", perfect3, best_sum3);
      }
    }
  }
  
  best_of_best[0] = chosen_population[0][best_gen1];
  best_of_best[1] = chosen_population[1][best_gen1];
  best_of_best[2] = chosen_population[2][best_gen1];
  best_of_best[3] = chosen_population[3][best_gen2];
  best_of_best[4] = chosen_population[4][best_gen2];
  best_of_best[5] = chosen_population[5][best_gen2];
  best_of_best[6] = chosen_population[6][best_gen3];
  best_of_best[7] = chosen_population[7][best_gen3];
  best_of_best[8] = chosen_population[8][best_gen3];
  
  for (j = 0; j < NUMBER_OF_CLASSES; j++){
    best_of_best[j].best_gena7 = best_gen1;
    best_of_best[j].best_gena8 = best_gen2;
    best_of_best[j].best_gena9 = best_gen3;
  }
  printf("\n\n");
  printf("  1  perfection: %d  sum: %d \n", perfect1, best_sum1);
  printf("  2  perfection: %d  sum: %d \n", perfect2, best_sum2);
  printf("  3  perfection: %d  sum: %d \n\n", perfect3, best_sum3);
}

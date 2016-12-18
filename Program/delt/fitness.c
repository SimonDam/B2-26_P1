#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "fitness.h"

/* Runs through a for loop for each grade (expecting there is 3 parallelclasses in each grade) Inside this loop a loop is run that calculates the fitness for each class 
   while also having the parallelclasses a parameter to check for identical lessons */
void calculate_fitness_all(individual **population, int h_classes[], class_info **class_data, requirements *requirements_classes){
  int i, j;
  for(j = 0; j < NUMBER_OF_CLASSES; j+=3){
    for(i = 0; i < SIZE_OF_POPULATION; i++){
      calculate_fitness_one(&(population[j][i]), &(population[j+2][i]), &(population[j+1][i]), h_classes, class_data, j, j+2, j+1, requirements_classes[j]);
      calculate_fitness_one(&(population[j+1][i]), &(population[j][i]), &(population[j][i+2]), h_classes, class_data, j+1, j, j+2, requirements_classes[j+1]);
      calculate_fitness_one(&(population[j+2][i]), &(population[j+1][i]), &(population[j][i]), h_classes, class_data, j+2, j+1, j, requirements_classes[j+2]);
    }
  }
}

/* Calculating fitness of individual_master*/
void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int h_classes[], class_info **class_data, int class_master, int class_other1, int class_other2, requirements requirements_class){
  

  /* Reset the fitness */
  individual_master->fitness = 1;
  int i, j, k;
  /* Lessons in row (breaks counts as a lesson)*/
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i += 2){
      if (individual_master->lesson_num[i][j] == individual_master->lesson_num[i+1][j]){
        individual_master->fitness += FITNESS_LESSONS_IN_ROW;
      }
    }
  }
  /* Parallel classes - lessons in a sync */
  int test_parallel_both = 0;
  individual_master->lessons_with_parallel = 0;
  individual_master->lessons_with_both = 0;

  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      if (individual_master->lesson_num[i][j] == individual_other1->lesson_num[i][j]){
        if (individual_master->lesson_num[i][j] != fri){
          individual_master->fitness += FITNESS_PARALEL_CLASS;
          individual_master->lessons_with_parallel++;
          test_parallel_both++;
        }
      }
      if (individual_master->lesson_num[i][j] == individual_other2->lesson_num[i][j]){
        if (individual_master->lesson_num[i][j] != fri){
          individual_master->fitness += FITNESS_PARALEL_CLASS;
          individual_master->lessons_with_parallel++;
          test_parallel_both++;
        }
      }
      if (test_parallel_both == 2){
        individual_master->lessons_with_both++;
      }
      test_parallel_both = 0;
    }
  }
  
  /* Heavy lessons after 11.20 is bad  */
  individual_master->heavy_lesson_after = 0;
  individual_master->heavy_lesson_before = 0;
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      for (k = 0; k < NUMBER_OF_HEAVY_LESSONS; k++){
        if (i > LESSON_OVER_MIDDAY){
          if (individual_master->lesson_num[i][j] == h_classes[k]){
            individual_master->fitness += FITNESS_HEAVY_LESSONS;
            individual_master->heavy_lesson_after++;
          }
        }
        else {
          if (individual_master->lesson_num[i][j] == h_classes[k]){
            individual_master->fitness += FITNESS_HEAVY_LESSONS_BEFORE;
            individual_master->heavy_lesson_before++;
          }
        }
      }
    }
  }

  /* Free lessons */
  int count_free_lessons = 0;
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      /* Need to be a free lesson - count op */
      if (individual_master->lesson_num[i][j] == fri){
        count_free_lessons++;
      }

      /* In the middle */
      if (i < 6){
        if (individual_master->lesson_num[i][j] == fri){
          individual_master->fitness += FITNESS_FREE_IN_MIDLE;
          
        }
      }
      if (i == 6){
        if ((individual_master->lesson_num[i][j] == fri) && (individual_master->lesson_num[i+1][j] != fri)){
          individual_master->fitness += FITNESS_FREE_IN_MIDLE;
        }
      }
      
      /* In the end */
      if ((individual_master->lesson_num[i][j] == fri) && (i == 7) ) {
        individual_master->fitness += FITNESS_BONUS_FREE_END;
        if (individual_master->lesson_num[i-1][j] == fri){
          individual_master->fitness += FITNESS_BONUS_FREE_END;
        }
      }
    }
  }
  
  /* Give punish for not having a free lesson */
  if (count_free_lessons < NUMBER_OF_FREE_REQ){
    individual_master->fitness += FITNESS_NO_FREE_TIME;
  }
  
  /* Teacher preparation */
  int count = 0, test = 0;
  char temp_teacher_name[TEACHER_NAME_MAX] = "TEMP";

  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSON_OVER_MIDDAY; i++){
      if (strcmp(class_data[class_master][individual_master->lesson_num[i][j]].teacher_name, temp_teacher_name) != 0){
        count++;
      }
      else {
        memset(temp_teacher_name, '\0', sizeof(temp_teacher_name));
        strcat(temp_teacher_name, class_data[class_master][individual_master->lesson_num[i][j]].teacher_name);
        count = 1;
      }

      if (count > 2){
        individual_master->fitness += FITNESS_TEACHER_PREPARATION;
      }
    }
  }

  /* Too many of the same in a row */
  count = 0;
  test = 0;

  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSON_OVER_MIDDAY; i++){
      if (test == individual_master->lesson_num[i][j]){
        count++;
      }
      else {
        test = individual_master->lesson_num[i][j];
        count = 1;
      }

      if (count > MAX_LESSONS_IN_ROW){
        individual_master->fitness += FITNESS_MANY_LESSONS_IN_ROW;
      }

    }
  }
  /* Reads all the requirements for the current class into temp variables */
  int temp_Dan_req = requirements_class.Dan_req;
  int temp_Mat_req = requirements_class.Mat_req;
  int temp_Eng_req = requirements_class.Eng_req;
  int temp_Tys_req = requirements_class.Tys_req;
  int temp_Fys_req = requirements_class.Fys_req;
  int temp_His_req = requirements_class.His_req;
  int temp_Sam_req = requirements_class.Sam_req;
  int temp_Val_req = requirements_class.Val_req;
  int temp_Geo_req = requirements_class.Geo_req;
  int temp_Bio_req = requirements_class.Bio_req;
  int temp_Gym_req = requirements_class.Gym_req;
  int temp_Rel_req = requirements_class.Rel_req;
  int temp_Pra_req = requirements_class.Pra_req;

  /* Everytime a specific lesson is met in the schedule, the remaining required lessons of that type is counted down by one*/
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      switch (individual_master->lesson_num[i][j]){
        case dan:
          temp_Dan_req--;
          break;
        case mat:
          temp_Mat_req--;
          break;
        case eng:
          temp_Eng_req--;
          break;
        case tys:
          temp_Tys_req--;
          break;
        case fys:
          temp_Fys_req--;
          break;
        case his:
          temp_His_req--;
          break;
        case sam:
          temp_Sam_req--;
          break;
        case val:
          temp_Val_req--;
          break;
        case geo:
          temp_Geo_req--;
          break;
        case bio:
          temp_Bio_req--;
          break;
        case gym:
          temp_Gym_req--;
          break;
        case rel:
          temp_Rel_req--;
          break;
        case pra:
          temp_Pra_req--;
          break;
        case fri:
          break;
        default:
          printf("ERROR IN FITNESS\n");
          exit(0);
      }
    }
  }

  /* If some requirements are not counted all the way down to 0, it means the class is missing specific lessons, and the fitness is being affected with the chosen
     penalty for not meeting requirements */
  int perfection_temp = 0;
  if (temp_Dan_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Dan_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Mat_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Mat_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Eng_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Eng_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Tys_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Tys_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Fys_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Fys_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_His_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_His_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Sam_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Sam_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Val_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Val_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Geo_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Geo_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Bio_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Bio_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Gym_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Gym_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Rel_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Rel_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }

  if (temp_Pra_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Pra_req < TOO_MANY_OF_LESSON){
    individual_master->fitness += FITNESS_WAY_OVER_REQ;
  }
  else {
    individual_master->fitness += FITNESS_CORRECT_LESSONS;
    perfection_temp++;
  }
  individual_master->perfection = perfection_temp;

  /* Perfecktion bonus */
  int y;
  for (y = 0; y < perfection_temp; y++){
    individual_master->fitness += FITNESS_PERFECTION_BONUS;
  }

  /* Teacher overbooked */
  individual_master->teacher_overbooked = 0;
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      if (strcmp(class_data[class_master][individual_master->lesson_num[i][j]].teacher_name, class_data[class_other1][individual_other1->lesson_num[i][j]].teacher_name) == 0){
        individual_master->fitness += FITNESS_TEACHER_OVERBOOKED;
        individual_master->teacher_overbooked += 1;
      }
      else {
        individual_master->fitness += FITNESS_NOT_OVERBOOKED;
      }

      if (strcmp(class_data[class_master][individual_master->lesson_num[i][j]].teacher_name, class_data[class_other2][individual_other2->lesson_num[i][j]].teacher_name) == 0){
        individual_master->fitness += FITNESS_TEACHER_OVERBOOKED;
        individual_master->teacher_overbooked += 1;
      }
      else {
        individual_master->fitness += FITNESS_NOT_OVERBOOKED;
      }
    }
  }

 /* If it is negative, change to one */
  if (individual_master->fitness < 1){
    individual_master->fitness = 1;
  }
}


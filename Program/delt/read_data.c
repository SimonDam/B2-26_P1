#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "read_data.h"


/* Reading info from file into arrays of struct */
void read_data(class_info **class_data){ 
  int i, j;
  class_info local_class_data;
  FILE *info = fopen("info.txt", "r");
  if(info == NULL){
    perror("Error the file is empty");
    fclose(info);
    exit(1);
  }

  for(j = 0; j < NUMBER_OF_CLASSES; j++){
    for(i = 0;i < NUMBER_OF_SUBJECTS; i++){
      fscanf(info,
      " %s %s %d %s ",
      local_class_data.teacher_name, 
      local_class_data.lesson_name, 
      &local_class_data.number_of_lessons, 
      local_class_data.class_name);
      class_data[j][i] = local_class_data;
    } 
  }  
  fclose(info);
}

/*Setting requirements into array struct*/
void find_req(class_info **class_data, requirements *requirements_classes){
  int j = 0, i  = 0, k = 0;
  for(j = 0, k = 0; j < NUMBER_OF_CLASSES; j++, k++){
    for(i = 0; i < NUMBER_OF_SUBJECTS; i ++){
      if(strcmp("Dan", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Dan_req = class_data[j][i].number_of_lessons;
      }
      else if(strcmp("Mat", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Mat_req = class_data[j][i].number_of_lessons;
      }
      else if(strcmp("Eng", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Eng_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Tys", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Tys_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Fys", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Fys_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("His", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].His_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Val", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Val_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Geo", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Geo_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Bio", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Bio_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Gym", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Gym_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Rel", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Rel_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Sam", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Sam_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Fri", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Fri_req = class_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Pra", class_data[j][i].lesson_name) == 0){
        requirements_classes[k].Pra_req = class_data[j][i].number_of_lessons;  
      }
    }
  }
}

/*creating schudules with random lessons */
void create_individuals(individual **population){
  int i, j;
  for (j = 0; j < NUMBER_OF_CLASSES; j++ ){
    for(i = 0; i < SIZE_OF_POPULATION; i++){
      population[j][i] = create_individual();
    }
  }
}

individual create_individual(void){
  /* Creating the individual */
  individual result;

  /* Making it random */
  int i, j;
  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      result.lesson_num[i][j] = rand() % NUMBER_OF_DIFFERENT_LESSONS;
    }
  }
  return result;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "read_data.h"


void read_teachers_name(teacher teacher_data[][NUMBER_OF_SUBJECTS]){ 
 FILE *teacherinfo = fopen("info.txt", "r");
  if(teacherinfo == NULL){
    perror("Error the file is empty");
    fclose(teacherinfo);
    exit(1);
  }

  teacher local_teacher_data;
  int i = 0, j = 0;
  for(j = 0; j < NUMBER_OF_CLASSES; j++){
    for(i = 0;i < NUMBER_OF_SUBJECTS; i++){
      fscanf(teacherinfo,
      " %s %s %d %s ",
      local_teacher_data.teacher_name, 
      local_teacher_data.lesson_name, 
      &local_teacher_data.number_of_lessons, 
      local_teacher_data.class_name);
      teacher_data[j][i] = local_teacher_data;
    } 
  }  
  fclose(teacherinfo);
}

void find_req(teacher teacher_data[][NUMBER_OF_SUBJECTS], requirements requirements_classes[]){
  int j = 0, i  = 0, k = 0;
  for(j = 0, k = 0; j < NUMBER_OF_CLASSES; j++, k++){
    for(i = 0; i < NUMBER_OF_SUBJECTS; i ++){
      if(strcmp("Dan", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Dan_req = teacher_data[j][i].number_of_lessons;
      }
      else if(strcmp("Mat", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Mat_req = teacher_data[j][i].number_of_lessons;
      }
      else if(strcmp("Eng", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Eng_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Tys", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Tys_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Fys", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Fys_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("His", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].His_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Val", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Val_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Geo", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Geo_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Bio", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Bio_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Gym", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Gym_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Rel", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Rel_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Sam", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Sam_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Fri", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Fri_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Pra", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[k].Pra_req = teacher_data[j][i].number_of_lessons;  
      }
    }
  }
}


void create_individuals(individual individuals[][NUMBER_OF_INDIVIDUALS]){
  for (int j = 0; j < NUMBER_OF_CLASSES; j++ ){
    for(int i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      individuals[j][i] = create_individual();
    }
  }
}

individual create_individual(){
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


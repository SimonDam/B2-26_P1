#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "print.h"

/*Finding best individuals from all generations*/
void find_best(individual chosen_individuals[][NUMBER_OF_GENERATIONS], individual best_of_best[]){
  printf("\n\n");
  int i, k, j;
  int best_sum1, best_sum2, best_sum3;
  int best_gen1, best_gen2, best_gen3;
  int sum1, sum2, sum3;
  int temp_perfect1 = 0, temp_perfect2 = 0, temp_perfect3 = 0;
  int perfect1 = 0, perfect2 = 0, perfect3 = 0;
  
  for (i = 0; i < NUMBER_OF_GENERATIONS; i++){
    /* Getting the sum */
    if ((chosen_individuals[0][i].fitness != 1) && (chosen_individuals[1][i].fitness != 1) && (chosen_individuals[2][i].fitness != 1)){
      sum1 = chosen_individuals[0][i].fitness + chosen_individuals[1][i].fitness + chosen_individuals[2][i].fitness;
    }
    else {
      sum1 = 0;
    }

    if ((chosen_individuals[3][i].fitness != 1) && (chosen_individuals[4][i].fitness != 1) && (chosen_individuals[5][i].fitness != 1)){
      sum2 = chosen_individuals[3][i].fitness + chosen_individuals[4][i].fitness + chosen_individuals[5][i].fitness;
    }
    else {
      sum2 = 0;
    }

    if ((chosen_individuals[6][i].fitness != 1) && (chosen_individuals[7][i].fitness != 1) && (chosen_individuals[8][i].fitness != 1)){
      sum3 = chosen_individuals[6][i].fitness + chosen_individuals[7][i].fitness + chosen_individuals[8][i].fitness;
    }
    else {
      sum3 = 0;
    }

    /* Getting the lowest perfection grade */
    temp_perfect1 = 15;
    temp_perfect2 = 15;
    temp_perfect3 = 15;

    if (temp_perfect1 > chosen_individuals[0][i].perfection){
      temp_perfect1 = chosen_individuals[0][i].perfection;
    }
    if (temp_perfect1 > chosen_individuals[1][i].perfection){
      temp_perfect1 = chosen_individuals[1][i].perfection;
    }
    if (temp_perfect1 > chosen_individuals[2][i].perfection){
      temp_perfect1 = chosen_individuals[2][i].perfection;
    }

    if (temp_perfect2 > chosen_individuals[3][i].perfection){
      temp_perfect2 = chosen_individuals[3][i].perfection;
    }
    if (temp_perfect2 > chosen_individuals[4][i].perfection){
      temp_perfect2 = chosen_individuals[4][i].perfection;
    }
    if (temp_perfect2 > chosen_individuals[5][i].perfection){
      temp_perfect2 = chosen_individuals[5][i].perfection;
    }

    if (temp_perfect3 > chosen_individuals[6][i].perfection){
      temp_perfect3 = chosen_individuals[6][i].perfection;
    }
    if (temp_perfect3 > chosen_individuals[7][i].perfection){
      temp_perfect3 = chosen_individuals[7][i].perfection;
    }
    if (temp_perfect3 > chosen_individuals[8][i].perfection){
      temp_perfect3 = chosen_individuals[8][i].perfection;
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
  
  best_of_best[0] = chosen_individuals[0][best_gen1];
  best_of_best[1] = chosen_individuals[1][best_gen1];
  best_of_best[2] = chosen_individuals[2][best_gen1];
  best_of_best[3] = chosen_individuals[3][best_gen2];
  best_of_best[4] = chosen_individuals[4][best_gen2];
  best_of_best[5] = chosen_individuals[5][best_gen2];
  best_of_best[6] = chosen_individuals[6][best_gen3];
  best_of_best[7] = chosen_individuals[7][best_gen3];
  best_of_best[8] = chosen_individuals[8][best_gen3];
  
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








void print_req(individual chosen_individual, requirements requirements_class){
  printf("Must have:\t %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                  requirements_class.Dan_req,
                  requirements_class.Mat_req,
                  requirements_class.Eng_req,
                  requirements_class.Tys_req,
                  requirements_class.Fys_req,
                  requirements_class.His_req,
                  requirements_class.Sam_req,
                  requirements_class.Val_req,
                  requirements_class.Geo_req,
                  requirements_class.Bio_req,
                  requirements_class.Gym_req,
                  requirements_class.Rel_req,
                  requirements_class.Pra_req,
                  requirements_class.Fri_req);


  int temp_Dan_req = 0;
  int temp_Mat_req = 0;
  int temp_Eng_req = 0;
  int temp_Tys_req = 0;
  int temp_Fys_req = 0;
  int temp_His_req = 0;
  int temp_Sam_req = 0;
  int temp_Val_req = 0;
  int temp_Geo_req = 0;
  int temp_Bio_req = 0;
  int temp_Gym_req = 0;
  int temp_Rel_req = 0;
  int temp_Pra_req = 0;
  int temp_Fri_req = 0;

  int i, j;
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      switch (chosen_individual.lesson_num[i][j]){
        case dan:
          temp_Dan_req++;
          break;
        case mat:
          temp_Mat_req++;
          break;
        case eng:
          temp_Eng_req++;
          break;
        case tys:
          temp_Tys_req++;
          break;
        case fys:
          temp_Fys_req++;
          break;
        case his:
          temp_His_req++;
          break;
        case sam:
          temp_Sam_req++;
          break;
        case val:
          temp_Val_req++;
          break;
        case geo:
          temp_Geo_req++;
          break;
        case bio:
          temp_Bio_req++;
          break;
        case fri:
          temp_Fri_req++;
          break;
        case gym:
          temp_Gym_req++;
          break;
        case rel:
          temp_Rel_req++;
          break;
        case pra:
          temp_Pra_req++;
          break;
        default:
          printf("ERROR IN FITNESS\n");
          break;
      }
    }
  }

  printf("Have: \t\t %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
          temp_Dan_req,
          temp_Mat_req,
          temp_Eng_req,
          temp_Tys_req,
          temp_Fys_req,
          temp_His_req,
          temp_Sam_req,
          temp_Val_req,
          temp_Geo_req,
          temp_Bio_req,
          temp_Gym_req,
          temp_Rel_req,
          temp_Pra_req,
          temp_Fri_req);
}
/*Printing the entire schudule*/
void print_func(individual best_of_best[], requirements requirements_classes[], int generation_print, teacher teacher_data[][NUMBER_OF_SUBJECTS]){
  int i, j, c;
  printf("\n\n   Teachers\n");
  for (i = 0; i < NUMBER_OF_CLASSES; i++){
    for (j = 0; j < NUMBER_OF_SUBJECTS; j++){
      printf("%s  %s %s\t", teacher_data[i][j].teacher_name, teacher_data[i][j].lesson_name, teacher_data[i][j].class_name);
    }
    printf("\n");
  }
  printf("\n\n");

  printf("taken from     7: %d  8: %d  9: %d  \n\n", best_of_best[0].best_gena7, best_of_best[0].best_gena8, best_of_best[0].best_gena9);

  for (c = 0; c < NUMBER_OF_CLASSES; c++){
    /* Printing testing */
    print_req(best_of_best[c], requirements_classes[c]);
    printf("Has a fitness of: %d   The perfection grade is: %d  Lessons with parallel: %d  Lessons With Both: %d  Heavy lessons after: %d Before: %d   Overbooked: %d \n\n", best_of_best[c].fitness, best_of_best[c].perfection, best_of_best[c].lessons_with_parallel, best_of_best[c].lessons_with_both, best_of_best[c].heavy_lesson_after, best_of_best[c].heavy_lesson_before, best_of_best[c].teacher_overbooked);

    /* Printing the days */
    printf("  Class: ");
    print_class_name(c);
    printf("\n");
    printf("  Tidspunkt\t\tMandag\t\tTirsdag\t\tOnsdag\t\tTorsdag\t\tFredag\n");
    printf("  ------------------------------------------------------------------------------------------------\n");
    
    for (i = 0; i < LESSONS_PER_DAY_MAX; i++){
      /* Printing the times */
      print_time_func(i);

      for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
        /* Printing lesson name */
        print_teacher_and_lesson(best_of_best[c].lesson_num[i][j], c, teacher_data);
        printf("\t\t");
      }
      /* To signal a break */
      if ((i+1) % 2 == 0){
        printf("\n");
      }
      printf("\n");
    }
    printf("\n\n\n");
  }
}
/*Printing time of day*/
void print_time_func (int i){
  if (i == 0){
    printf("   8.00 -  8.45  |\t");
  }
  else if(i == 1){
    printf("   8.45 -  9.30  |\t");
  }
  else if(i == 2){
    printf("   9.50 - 10.35  |\t");
  }
  else if(i == 3){
    printf("  10.35 - 11.20  |\t");
  }
  else if(i == 4){
    printf("  11.50 - 12.35  |\t");
  }
  else if(i == 5){
    printf("  12.35 - 13.20  |\t");
  }
  else if(i == 6){
    printf("  13.30 - 14.15  |\t");
  }
  else if(i == 7){
    printf("  14.15 - 15.00  |\t");
  }
}

/*Printing subjekt and teacher name*/
void print_teacher_and_lesson(int subject, int class, teacher teacher_data[][NUMBER_OF_SUBJECTS]){
  /* Saving the name of subjekt */
  char subject_name[LESSON_NAME_MAX];
  memset(subject_name, '\0', LESSON_NAME_MAX);
  switch (subject){
    case dan:
      strcpy(subject_name, "Dan");
      break;
    case mat:
      strcpy(subject_name, "Mat");
      break;
    case eng:
      strcpy(subject_name, "Eng");
      break;
    case tys:
      strcpy(subject_name, "Tys");
      break;
    case fys:
      strcpy(subject_name, "Fys");
      break;
    case his:
      strcpy(subject_name, "His");
      break;
    case sam:
      strcpy(subject_name, "Sam");
      break;
    case val:
      strcpy(subject_name, "Val");
      break;
    case geo:
      strcpy(subject_name, "Geo");
      break;
    case bio:
      strcpy(subject_name, "Bio");
      break;
    case gym:
      strcpy(subject_name, "Gym");
      break;
    case fri:
      strcpy(subject_name, "---");
      break;
    case rel:
      strcpy(subject_name, "Rel");
      break;
    case pra:
      strcpy(subject_name, "Pra");
      break;
    default:
      printf("ERROR\n");
      break;
  }

  /* Finding the class and subject. And printing the name */
  int i;
  for (i = 0; i < NUMBER_OF_SUBJECTS; i++){
    switch (class){
      case 0:
        if (strcmp("7a", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 1:
        if (strcmp("7b", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 2:
        if (strcmp("7c", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 3:
        if (strcmp("8a", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 4:
        if (strcmp("8b", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 5:
        if (strcmp("8c", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 6:
        if (strcmp("9a", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 7:
        if (strcmp("9b", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      case 8:
        if (strcmp("9c", teacher_data[class][i].class_name) == 0){
          if (strcmp(subject_name, teacher_data[class][i].lesson_name) == 0){
            printf("%s", teacher_data[class][i].teacher_name);
          }
        }
        break;
      default:
        printf("ERROR");
        break; 
    }
  }
  /* Printing the lesson */
  printf("  %s", subject_name);
}

/*Printing class name*/
void print_class_name(int c){
  switch (c){
    case 0:
      printf("7.A");
      break;
    case 1:
      printf("7.B");
      break;
    case 2:
      printf("7.C");
      break;
    case 3:
      printf("8.A");
      break;
    case 4:
      printf("8.B");
      break;
    case 5:
      printf("8.C");
      break;
    case 6:
      printf("9.A");
      break;
    case 7:
      printf("9.B");
      break;
    case 8:
      printf("9.C");
      break;
    default:
      printf("ERROR");
      break;
  }
}
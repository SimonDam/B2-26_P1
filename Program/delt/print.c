#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "print.h"

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
void print_func(individual best_of_best[], requirements requirements_classes[], class_info **class_data){
  int i, j, c;
  printf("\n\n   teachers\n");
  for (i = 0; i < NUMBER_OF_CLASSES; i++){
    for (j = 0; j < NUMBER_OF_SUBJECTS; j++){
      printf("%s  %s %s\t", class_data[i][j].teacher_name, class_data[i][j].lesson_name, class_data[i][j].class_name);
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
        print_teacher_and_lesson(best_of_best[c].lesson_num[i][j], c, class_data);
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
void print_teacher_and_lesson(int subject, int class, class_info **class_data){
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
        if (strcmp("7a", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 1:
        if (strcmp("7b", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 2:
        if (strcmp("7c", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 3:
        if (strcmp("8a", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 4:
        if (strcmp("8b", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 5:
        if (strcmp("8c", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 6:
        if (strcmp("9a", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 7:
        if (strcmp("9b", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
          }
        }
        break;
      case 8:
        if (strcmp("9c", class_data[class][i].class_name) == 0){
          if (strcmp(subject_name, class_data[class][i].lesson_name) == 0){
            printf("%s", class_data[class][i].teacher_name);
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
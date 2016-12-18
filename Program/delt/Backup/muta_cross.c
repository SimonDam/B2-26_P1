#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "defines.h"
#include "muta_cross.h"

/*Creating SIZE_OF_POPULATION numbers of children which is created from random choosen parents from old_population*/
void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], requirements requirements_classes[], int generation){
  int k, j;
  int rand_parent1 = 0, rand_parent2 = 0;
  
  make_old_population(individuals, individuals_temp);
  for(j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for(k = 0; k < NUMBER_OF_INDIVIDUALS; k++){
      /*Choosing random parents*/
      rand_parent1 = rand()% NUMBER_OF_INDIVIDUALS;
      rand_parent2 = rand()% NUMBER_OF_INDIVIDUALS;
      crossover_indi(individuals, individuals_temp[j][rand_parent1], individuals_temp[j][rand_parent2], requirements_classes[j], j, k, generation);
      crossover_indi(individuals, individuals_temp[j+1][rand_parent1], individuals_temp[j+1][rand_parent2], requirements_classes[j+1], j+1, k, generation);
      crossover_indi(individuals, individuals_temp[j+2][rand_parent1], individuals_temp[j+2][rand_parent2], requirements_classes[j+2], j+2, k, generation);
    }
  }
} 

/*Creating a child using 2 parents. Child day 1 = random day from parent 1. Child day 2 = random day from parent 2. 
Child day 3 and 4 mixing 2 random days(different from the used random days) from parent 1 and 2 each and mixing them together.
Child day 5 is filled with lessons to meet requirements. When requirements is meet the rest is fri.*/
void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num, int generation){
  int i, k, j;
  //int classes[1][NUMBER_OF_SUBJECTS];
  int chance = 0, day1_parent1 = 0, day1_parent2 = 0, day2_parent1 = 0, day2_parent2 = 0, day3_parent1 = 0, day3_parent2 = 0, ran_lesson = 0;
  /*Allocating space for req_temp to count requirements*/
  requirements *requirements_temp;
  requirements_temp = (requirements *)calloc(1 , sizeof(requirements));
  /*reseting child using fri lesson*/
  for(k = 0; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].lesson_num[k][0] = fri;
    individuals[class][indi_num].lesson_num[k][1] = fri;
    individuals[class][indi_num].lesson_num[k][2] = fri;
    individuals[class][indi_num].lesson_num[k][3] = fri;
    individuals[class][indi_num].lesson_num[k][4] = fri;
  } 
  /*Choosing random days*/
  day1_parent1 = rand()% SCHOOL_DAYS_IN_WEEK;
  day1_parent2 = rand()% SCHOOL_DAYS_IN_WEEK;
  while(day2_parent1 == day1_parent1){
    day2_parent1 = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  while(day2_parent2 == day1_parent2){
    day2_parent2 = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  while(day3_parent1 == day1_parent1 || day3_parent1 == day2_parent1){
    day3_parent1 = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  while(day3_parent2 == day1_parent2 || day3_parent2 == day2_parent2){
    day3_parent2 = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  for(k = 0; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].lesson_num[k][0] = parent1.lesson_num[k][day1_parent1];
    individuals[class][indi_num].lesson_num[k][1] = parent2.lesson_num[k][day1_parent2];
  } 

  ran_lesson = rand()% LESSONS_PER_DAY_MAX;
  for(k = 0; k < ran_lesson; k++){   
    individuals[class][indi_num].lesson_num[k][2] = parent2.lesson_num[k][day3_parent2];
    individuals[class][indi_num].lesson_num[k][3] = parent1.lesson_num[k][day3_parent1];
  }
  for(k = ran_lesson; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].lesson_num[k][2] = parent1.lesson_num[k][day2_parent1];
    individuals[class][indi_num].lesson_num[k][3] = parent2.lesson_num[k][day2_parent2];
  }

  /*Counting lessons in child so far */
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK-1; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      switch (individuals[class][indi_num].lesson_num[i][j]){
        case dan:
          requirements_temp->Dan_req++;
          break;
        case mat:
          requirements_temp->Mat_req++;
          break;
        case eng:
          requirements_temp->Eng_req++;
          break;
        case tys:
          requirements_temp->Tys_req++;
          break;
        case fys:
          requirements_temp->Fys_req++;
          break;
        case his:
          requirements_temp->His_req++;
          break;
        case sam:
          requirements_temp->Sam_req++;
          break;
        case val:
          requirements_temp->Val_req++;
          break;
        case geo:
          requirements_temp->Geo_req++;
          break;
        case bio:
          requirements_temp->Bio_req++;
          break;
        case fri:
          requirements_temp->Fri_req++;
          break;
        case gym:
          requirements_temp->Gym_req++;
          break;
        case rel:
          requirements_temp->Rel_req++;
          break;
        case pra:
          requirements_temp->Pra_req++;
          break;
        default:
          printf("ERROR IN FITNESS\n");
          break;
      }
    }
  }

  /*Comparing lessons so far to requirements and filling day 5 if it's needed*/
  chance = 0;
  for(i = 0, k = 0; i < 400; i++){
    if(k == 8){
      break;
    }
    chance = rand()% NUMBER_OF_SUBJECTS;
    switch(chance){
      case dan:
        if(requirements_temp->Dan_req < requirements_classes.Dan_req){
          individuals[class][indi_num].lesson_num[k][4] = dan;
          requirements_temp->Dan_req++;
          k++;
        }
        break;
      case mat:
        if(requirements_temp->Mat_req < requirements_classes.Mat_req){
          individuals[class][indi_num].lesson_num[k][4] = mat;
          requirements_temp->Mat_req++;
          k++;
        }
        break;
      case eng:
        if(requirements_temp->Eng_req < requirements_classes.Eng_req){
          individuals[class][indi_num].lesson_num[k][4] = eng;
          requirements_temp->Eng_req++;
          k++;
        }
       break;
      case tys:
        if(requirements_temp->Tys_req < requirements_classes.Tys_req){
          individuals[class][indi_num].lesson_num[k][4] = tys;
          requirements_temp->Tys_req++;
          k++;
        }           
        break;
      case fys:
        if(requirements_temp->Fys_req < requirements_classes.Fys_req){
          individuals[class][indi_num].lesson_num[k][4] = fys;
          requirements_temp->Fys_req++;
          k++;
        }
       break;
      case his:
        if(requirements_temp->His_req < requirements_classes.His_req){
          individuals[class][indi_num].lesson_num[k][4] = his;
          requirements_temp->His_req++;
          k++;
        }
        break;
      case sam:
        if(requirements_temp->Sam_req < requirements_classes.Sam_req){
          individuals[class][indi_num].lesson_num[k][4] = sam;
          requirements_temp->Sam_req++;
          k++;
        }
        break;
      case val:
        if(requirements_temp->Val_req < requirements_classes.Val_req){
          individuals[class][indi_num].lesson_num[k][4] = val;
          requirements_temp->Val_req++;
          k++;
        }
        break;
      case geo:
        if(requirements_temp->Geo_req < requirements_classes.Geo_req){
          individuals[class][indi_num].lesson_num[k][4] = geo;
          requirements_temp->Geo_req++;
          k++;
        }
        break;
      case bio:
        if(requirements_temp->Bio_req < requirements_classes.Bio_req){
          individuals[class][indi_num].lesson_num[k][4] = bio;
          requirements_temp->Bio_req++;
          k++;
        }
        break;
      case gym:
        if(requirements_temp->Gym_req < requirements_classes.Gym_req){
          individuals[class][indi_num].lesson_num[k][4] = gym;
          requirements_temp->Gym_req++;
          k++;
        }
        break;
      case rel:
        if(requirements_temp->Rel_req < requirements_classes.Rel_req){
          individuals[class][indi_num].lesson_num[k][4] = rel;
          requirements_temp->Rel_req++;
          k++;
        }
        break;
      case pra:
        if(requirements_temp->Pra_req < requirements_classes.Pra_req){
          individuals[class][indi_num].lesson_num[k][4] = pra;
          requirements_temp->Pra_req++;
          k++;
        }
        break;
      default:
        break;
    }
  }



  free(requirements_temp);
}

/*Putting current population over in old population*/
void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]){
  int j, i;  
  for(j = 0; j < NUMBER_OF_CLASSES; j++){ 
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      individuals_temp[j][i] = individuals[j][i];
    }
  }
}

/* A for loop is run as many times as the possible mutations is set. Each time the loop is run, there is a specific chance (CHANCE_OF_MUTATION) that mutations
   will ocour. When mutation occours, program picks 2 random days that are not the same and 2 random lessons that are not the same. the first random lesson 
   in the first random day is then swapped with the second random lesson in the second random day S*/
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]){
  int i, j, k;
  int ran1Day = 0, ran1Week = 0, ran2Day = 0, ran2Week = 0, chance = 0, mutations = 0, temp = 0;
  for(k = 0; k < NUMBER_OF_CLASSES; k++){
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      mutations = rand()% (MAX_MUTATIONS_PER_INDIVIDUAL+1);
      for (j = 0; j < mutations; j++){
        chance = rand()% 100;
        if (chance < CHANCE_OF_MUTATION){
          do {
            ran1Week = rand()% SCHOOL_DAYS_IN_WEEK;
            ran1Day = rand()% LESSONS_PER_DAY_MAX;
            ran2Week = rand()% SCHOOL_DAYS_IN_WEEK;
            ran2Day = rand()% LESSONS_PER_DAY_MAX;
          } while ((ran1Week == ran2Week) && (ran1Day == ran2Day));
        
          temp = individuals[k][i].lesson_num[ran1Day][ran1Week];
          individuals[k][i].lesson_num[ran1Day][ran1Week] = individuals[j][i].lesson_num[ran2Day][ran2Week];
          individuals[k][i].lesson_num[ran2Day][ran2Week] = temp;
        }
      }
    }
  }
}
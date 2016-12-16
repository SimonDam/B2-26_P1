#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define LESSON_NAME_MAX 12
#define SCHOOL_DAYS_IN_WEEK 5
#define TEACHER_NAME_MAX 5
#define LESSONS_PER_DAY_MAX 8
#define LESSONS_PER_WEEK_MAX (LESSONS_PER_DAY_MAX * SCHOOL_DAYS_IN_WEEK)
#define NUMBER_OF_DIFFERENT_LESSONS 14
#define DEFAULT_LENGTH_STRING 200
#define LESSON_OVER_MIDDAY 3
#define NUMBER_OF_HEAVY_LESSONS 5
#define NUMBER_OF_CLASSES 9
#define NUMBER_OF_SUBJECTS 13
#define TEACHER_FREE_BEFORE_POINT 2
#define TOO_MANY_OF_LESSON -2

#define MAX_LESSONS_IN_ROW 3

#define NUMBER_OF_FREE_REQ 4

#define NUMBER_OF_INDIVIDUALS 100
#define NUMBER_OF_GENERATIONS 800

#define CHANCE_OF_MUTATION 20
#define MAX_MUTATIONS_PER_INDIVIDUAL 8

#define FITNESS_LESSONS_IN_ROW 80
#define FITNESS_PARALEL_CLASS 100
#define FITNESS_HEAVY_LESSONS -200
#define FITNESS_HEAVY_LESSONS_BEFORE 200
#define FITNESS_FREE_IN_MIDLE -100000
#define FITNESS_MANY_LESSONS_IN_ROW -500
#define FITNESS_TEACHER_OVERBOOKED -1000
#define FITNESS_TEACHER_PREPARATION 60
#define FITNESS_NOT_MEET_REQ -60
#define FITNESS_WAY_OVER_REQ -600
#define FITNESS_CORRECT_LESSONS 80
#define FITNESS_BONUS_FREE_END 50
#define FITNESS_PERFECTION_BONUS 200
#define FITNESS_NO_FREE_TIME -100
#define FITNESS_NOT_OVERBOOKED 50

enum lesson_number {dan, mat, eng, tys, fys, his, sam, val, geo, bio, gym, rel, pra, fri};

typedef struct individual individual;
struct individual{
  int lesson_num[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  int fitness;
  int perfection;
  int lessons_with_parallel;
  int lessons_with_both;
  int heavy_lesson_after;
  int heavy_lesson_before;
  int teacher_overbooked;
  int best_gena7;
  int best_gena8;
  int best_gena9;
};

typedef struct teacher teacher;
struct teacher{
  char teacher_name[TEACHER_NAME_MAX];
  char lesson_name[LESSON_NAME_MAX];
  int number_of_lessons;
  char class_name[TEACHER_NAME_MAX];
};

typedef struct requirements requirements;
struct requirements{
  int Dan_req;
  int Mat_req;
  int Eng_req;
  int Tys_req;
  int Fys_req;
  int His_req;
  int Sam_req;
  int Val_req;
  int Geo_req;
  int Bio_req;
  int Gym_req;
  int Fri_req;
  int Rel_req;
  int Pra_req;
};

int find_number_of_classes(teacher teacher_data[], int number_of_teacher);
void read_teachers_name(teacher teacher_data[][NUMBER_OF_SUBJECTS]);
void find_req(teacher teacher_data[][NUMBER_OF_SUBJECTS], requirements requirements_classes[]);
void create_individuals(individual individuals[][NUMBER_OF_INDIVIDUALS]);
individual create_individual();
void selektion(individual individuals[][NUMBER_OF_INDIVIDUALS]);
int pick_individual(individual temp_individuals[][NUMBER_OF_INDIVIDUALS], individual individuals[][NUMBER_OF_INDIVIDUALS], int class, int lesson_number);
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]);
void calculate_fitness_all(individual individuals[][NUMBER_OF_INDIVIDUALS], int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], requirements requirements_classes[]);
void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], int class_master, int class_other1, int class_other2, requirements requirements_class);
void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]);
void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], requirements requirements_classes[], int generation);
void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num, int generation);
void choose_individual(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_chosen[][NUMBER_OF_GENERATIONS], int class, int generation);
void calculate_finess_parallel(individual individuals[][NUMBER_OF_INDIVIDUALS], int sum_parrallel[], int class);
void find_best(individual chosen_individuals[][NUMBER_OF_GENERATIONS], individual best_of_best[]);
void print_func(individual best_of_best[], requirements requirements_class[], int generation_print, teacher teacher_data[][NUMBER_OF_SUBJECTS]);
void print_teacher_and_lesson(int subject, int class, teacher teacher_data[][NUMBER_OF_SUBJECTS]);
void print_time_func (int i);
void print_class_name(int c);
void print_req(individual chosen_individual, requirements requirements_class);

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

 for(int j = 0; j < NUMBER_OF_CLASSES; j += 3){
    choose_individual(individuals, chosen_individual, j, 0);
  }
 
  /* Genetic algorithem */
  int i, j;
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

void find_best(individual chosen_individuals[][NUMBER_OF_GENERATIONS], individual best_of_best[]){
  printf("\n\n");
  int i, k, j = 0;
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


int find_number_of_classes(teacher teacher_data[], int number_of_teacher){
  int i = 0, k = 0;
  int differrent_classes = 0;
  char grade[LESSON_NAME_MAX] = "1a";
  for(i = 0; i < number_of_teacher; i++){
    for(k = 0; k < number_of_teacher; k++){
      if (strcmp(grade, teacher_data[i].class_name) == 0){
        differrent_classes++;
        grade[1]++;
      }
      else if(strcmp(grade, teacher_data[i].class_name) != 0){
        grade[0]++;
      }
    }  
  }
  return differrent_classes-1;
}

void read_teachers_name(teacher teacher_data[][NUMBER_OF_SUBJECTS]){ 
 FILE *teacherinfo = fopen("teacherinfo.txt", "r");
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


/* A for loop is run as many times as the possible mutations is set. Each time the loop is run, there is a specific chance (CHANCE_OF_MUTATION) that mutations
   will ocour. When mutation occours, program picks 2 random days that are not the same and 2 random lessons that are not the same. the first random lesson 
   in the first random day is then swapped with the second random lesson in the second random day S*/
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]){
  int i = 0, j = 0, k =0, ran1Day = 0, ran1Week = 0, ran2Day = 0, ran2Week = 0, chance = 0, mutations = 0, temp = 0;
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

/* Runs through a for loop for each grade (expecting there is 3 parallelclasses in each grade) Inside this loop a loop is run that calculates the fitness for each class 
   while also having the parallelclasses a parameter to check for identical lessons */
void calculate_fitness_all(individual individuals[][NUMBER_OF_INDIVIDUALS], int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], requirements requirements_classes[]){
  int i, j;
  for(j = 0; j < NUMBER_OF_CLASSES; j+=3){
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      calculate_fitness_one(&(individuals[j][i]), &(individuals[j+2][i]), &(individuals[j+1][i]), h_classes, teacher_data, j, j+2, j+1, requirements_classes[j]);
      calculate_fitness_one(&(individuals[j+1][i]), &(individuals[j][i]), &(individuals[j][i+2]), h_classes, teacher_data, j+1, j, j+2, requirements_classes[j+1]);
      calculate_fitness_one(&(individuals[j+2][i]), &(individuals[j+1][i]), &(individuals[j][i]), h_classes, teacher_data, j+2, j+1, j, requirements_classes[j+2]);
    }
  }
}

/*  */
void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], int class_master, int class_other1, int class_other2, requirements requirements_class){
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
      if (strcmp(teacher_data[class_master][(individual_master->lesson_num[i][j])].teacher_name, temp_teacher_name) != 0){
        count++;
      }
      else {
        memset(temp_teacher_name, '\0', sizeof(temp_teacher_name));
        strcat(temp_teacher_name, teacher_data[class_master][individual_master->lesson_num[i][j]].teacher_name);
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
  int temp_Fri_req = requirements_class.Fri_req;
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
          break;
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
      if (strcmp(teacher_data[class_master][individual_master->lesson_num[i][j]].teacher_name, teacher_data[class_other1][individual_other1->lesson_num[i][j]].teacher_name) == 0){
        individual_master->fitness += FITNESS_TEACHER_OVERBOOKED;
        individual_master->teacher_overbooked += 1;
      }
      else {
        individual_master->fitness += FITNESS_NOT_OVERBOOKED;
      }

      if (strcmp(teacher_data[class_master][individual_master->lesson_num[i][j]].teacher_name, teacher_data[class_other2][individual_other2->lesson_num[i][j]].teacher_name) == 0){
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
void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], requirements requirements_classes[], int generation){
  int i = 0, rand_parent1 = 0, rand_parent2 = 0, k = 0, j = 0, g = 0, h = 0;
  
  make_old_population(individuals, individuals_temp);
  for(j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for(k = 0; k < NUMBER_OF_INDIVIDUALS; k++){
      rand_parent1 = rand()% NUMBER_OF_INDIVIDUALS;
      rand_parent2 = rand()% NUMBER_OF_INDIVIDUALS;
      crossover_indi(individuals, individuals_temp[j][rand_parent1], individuals_temp[j][rand_parent2], requirements_classes[j], j, k, generation);
      crossover_indi(individuals, individuals_temp[j+1][rand_parent1], individuals_temp[j+1][rand_parent2], requirements_classes[j+1], j+1, k, generation);
      crossover_indi(individuals, individuals_temp[j+2][rand_parent1], individuals_temp[j+2][rand_parent2], requirements_classes[j+2], j+2, k, generation);
    }
  }
} 

void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num, int generation){
  int i = 0, ran1day = 0, ran2day = 0, ran3day = 0, ran4day = 0, ran5day = 0, ran6day = 0, ran_lesson = 0, k = 0, j = 0;
  int classes[1][NUMBER_OF_SUBJECTS];
  int chance = 0;

  requirements *requirements_temp;
  requirements_temp = (requirements *)calloc(1 , sizeof(requirements));

  for(k = 0; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].lesson_num[k][0] = fri;
    individuals[class][indi_num].lesson_num[k][1] = fri;
    individuals[class][indi_num].lesson_num[k][2] = fri;
    individuals[class][indi_num].lesson_num[k][3] = fri;
    individuals[class][indi_num].lesson_num[k][4] = fri;
  } 

  ran1day = rand()% SCHOOL_DAYS_IN_WEEK;
  ran2day = rand()% SCHOOL_DAYS_IN_WEEK;
  while(ran3day == ran1day){
    ran3day = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  while(ran4day == ran2day){
    ran4day = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  while(ran5day == ran1day || ran5day == ran3day){
    ran5day = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  while(ran6day == ran2day || ran6day == ran4day){
    ran6day = rand()% SCHOOL_DAYS_IN_WEEK;
  }
  for(k = 0; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].lesson_num[k][0] = parent1.lesson_num[k][ran1day];
    individuals[class][indi_num].lesson_num[k][1] = parent2.lesson_num[k][ran2day];
  } 

  ran_lesson = rand()% LESSONS_PER_DAY_MAX;
  for(k = 0; k < ran_lesson; k++){   
    individuals[class][indi_num].lesson_num[k][2] = parent2.lesson_num[k][ran6day];
    individuals[class][indi_num].lesson_num[k][3] = parent1.lesson_num[k][ran5day];
  }
  for(k = ran_lesson; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].lesson_num[k][2] = parent1.lesson_num[k][ran3day];
    individuals[class][indi_num].lesson_num[k][3] = parent2.lesson_num[k][ran4day];
  }

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

void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]){
  int j = 0, i = 0;  
  for(j = 0; j < NUMBER_OF_CLASSES; j++){ 
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      individuals_temp[j][i] = individuals[j][i];
    }
  }
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

/*
Gamle lærer:

CA Dan 6 7a
JA Mat 4 7a
HA Eng 3 7a
RA Tys 3 7a
MA Fys 2 7a
MA His 2 7a
KA Sam 2 7a
UA Val 2 7a
RA Geo 2 7a
MA Bio 2 7a
WA Gym 2 7a
DA Rel 0 7a
UA Pra 2 7a
CA Dan 6 7b
JB Mat 4 7b
HB Eng 3 7b
RB Tys 3 7b
MB Fys 2 7b
MB His 2 7b
KB Sam 2 7b
UB Val 2 7b
RB Geo 2 7b
MB Bio 2 7b
WB Gym 2 7b
DB Rel 0 7b
UB Pra 2 7b
CC Dan 6 7c
JC Mat 4 7c
HC Eng 3 7c
RC Tys 3 7c
MC Fys 2 7c
MC His 2 7c
KC Sam 2 7c
UC Val 2 7c
RC Geo 2 7c
MC Bio 2 7c
WC Gym 2 7c
DC Rel 0 7c
UC Pra 2 7c
CB Dan 6 8a
JD Mat 4 8a
HC Eng 3 8a
RC Tys 3 8a
MD Fys 2 8a
MD His 2 8a
KD Sam 2 8a
UD Val 2 8a
RD Geo 1 8a
MD Bio 2 8a
WD Gym 2 8a
DD Rel 1 8a
UD Pra 0 8a
CB Dan 6 8b
JA Mat 4 8b
HA Eng 3 8b
RB Tys 3 8b
MB Fys 2 8b
ME His 2 8b
KE Sam 2 8b
UE Val 2 8b
RE Geo 1 8b
ME Bio 2 8b
WE Gym 2 8b
DE Rel 1 8b
UE Pra 0 8b
CF Dan 6 8c
JF Mat 4 8c
HF Eng 3 8c
RB Tys 3 8c
MB Fys 2 8c
MF His 2 8c
KF Sam 2 8c
UF Val 2 8c
RF Geo 1 8c
MF Bio 2 8c
WF Gym 2 8c
DF Rel 1 8c
UF Pra 0 8c
CR Dan 6 9a
JR Mat 4 9a
HR Eng 3 9a
RR Tys 3 9a
MF Fys 3 9a
ME His 1 9a
KF Sam 2 9a
UR Val 2 9a
RR Geo 1 9a
MR Bio 1 9a
WR Gym 2 9a
DR Rel 1 9a
UR Pra 0 9a
CS Dan 6 9b
JS Mat 4 9b
HS Eng 3 9b
RS Tys 3 9b
MS Fys 3 9b
MS His 1 9b
KS Sam 2 9b
US Val 2 9b
RS Geo 1 9b
MS Bio 1 9b
WS Gym 2 9b
DS Rel 1 9b
US Pra 0 9b
CA Dan 6 9c
JB Mat 4 9c
HC Eng 3 9c
RT Tys 3 9c
MT Fys 3 9c
MT His 1 9c
KT Sam 2 9c
UC Val 2 9c
RF Geo 1 9c
ME Bio 1 9c
WB Gym 2 9c
DT Rel 1 9c
UT Pra 0 9c

*/
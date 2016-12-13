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
#define LESSON_OVER_MIDDAY 4
#define NUMBER_OF_HEAVY_LESSONS 5
#define NUMBER_OF_CLASSES 9
#define NUMBER_OF_SUBJECTS 13
#define TEACHER_FREE_BEFORE_POINT 2
#define TOO_MANY_OF_LESSON -2

#define MAX_LESSONS_IN_ROW 3

#define NUMBER_OF_FREE_REQ 2

#define NUMBER_OF_INDIVIDUALS 100
#define NUMBER_OF_GENERATIONS 200

#define CHANCE_OF_MUTATION 20
#define MAX_MUTATIONS_PER_INDIVIDUAL 5

#define FITNESS_LESSONS_IN_ROW 50
#define FITNESS_PARALEL_CLASS 50
#define FITNESS_HEAVY_LESSONS -2
#define FITNESS_FREE_IN_MIDLE -10000
#define FITNESS_MANY_LESSONS_IN_ROW -50
#define FITNESS_TEACHER_OVERBOOKED -100
#define FITNESS_TEACHER_PREPARATION 60
#define FITNESS_NOT_MEET_REQ -60
#define FITNESS_WAY_OVER_REQ -60
#define FITNESS_CORRECT_LESSONS 80
#define FITNESS_BONUS_FREE_END 200000
#define FITNESS_PERFECTION_BONUS 100
#define FITNESS_NO_FREE_TIME -100

enum lesson_number {dan, mat, eng, tys, fys, his, sam, val, geo, bio, gym, fri, rel, pra};

typedef struct lesson lesson;
struct lesson{
  char teacher_name[TEACHER_NAME_MAX];
  char lesson_name[LESSON_NAME_MAX];
};

typedef struct individual individual;
struct individual{
  int individual_num[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  int fitness;
  int grade;
  int perfection;
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
int find_number_of_teachers();
void read_teachers_name(teacher teacher_data[][NUMBER_OF_SUBJECTS], int number_teachers);
void find_req(teacher teacher_data[][NUMBER_OF_SUBJECTS], requirements requirements_classes[]);
void create_individuals(individual individuals[][NUMBER_OF_INDIVIDUALS]);
individual create_individual();
void selektion(individual individuals[][NUMBER_OF_INDIVIDUALS]);
int pick_individual(individual temp_individuals[][NUMBER_OF_INDIVIDUALS], individual individuals[][NUMBER_OF_INDIVIDUALS], int class, int individual_number);
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]);
void calculate_fitness_all(individual individuals[][NUMBER_OF_INDIVIDUALS], int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], requirements requirements_classes[]);
void calculate_fitness_one(individual *individual_master, individual *individual_parallel1, individual *individual_parallel2, int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], int class_master, int class_other1, int class_other2, requirements requirements_class);
void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]);
void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], requirements requirements_classes[], int generation);
void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num, int generation);
void choose_individual(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_chosen[][NUMBER_OF_GENERATIONS], int class, int generation);
void calculate_finess_parallel(individual individuals[][NUMBER_OF_INDIVIDUALS], int sum_parrallel[], int class);
void print_func(individual chosen_individual[][NUMBER_OF_GENERATIONS]);
void print_time_func (int i);
void print_class_name(int c);
void print_lesson_name(int num);


/*'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/
int main(void){
  /* init general stuff */
  int h_classes[NUMBER_OF_HEAVY_LESSONS] = {mat, fys, eng, dan, tys};

  /* init klasses */
  individual individuals[NUMBER_OF_CLASSES][NUMBER_OF_INDIVIDUALS];
  individual chosen_individual[NUMBER_OF_CLASSES][NUMBER_OF_GENERATIONS];
  individual individuals_temp[NUMBER_OF_CLASSES][NUMBER_OF_INDIVIDUALS];
  lesson week[NUMBER_OF_CLASSES][SCHOOL_DAYS_IN_WEEK * LESSONS_PER_DAY_MAX];
  requirements requirements_classes[NUMBER_OF_CLASSES];
  
  /*Finds number of teacher and reads in info*/
  int number_of_teacher = find_number_of_teachers();
  teacher teacher_data[NUMBER_OF_CLASSES][NUMBER_OF_SUBJECTS];
  read_teachers_name(teacher_data, number_of_teacher);
  find_req(teacher_data, requirements_classes);

  /* Init genetic algorithem */
  srand(time(NULL));

  create_individuals(individuals);
  calculate_fitness_all(individuals, h_classes, teacher_data, requirements_classes);

 for(int j = 0; j < (NUMBER_OF_CLASSES); j += 3){
      choose_individual(individuals, chosen_individual, j, 0);  
  
    printf("%d   %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \n",0 , chosen_individual[0][0].fitness, chosen_individual[1][0].fitness, chosen_individual[2][0].fitness, chosen_individual[3][0].fitness, chosen_individual[4][0].fitness, chosen_individual[5][0].fitness, chosen_individual[6][0].fitness, chosen_individual[7][0].fitness, chosen_individual[8][0].fitness);
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
    printf("%d   %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \n",i , chosen_individual[0][i].fitness, chosen_individual[1][i].fitness, chosen_individual[2][i].fitness, chosen_individual[3][i].fitness, chosen_individual[4][i].fitness, chosen_individual[5][i].fitness, chosen_individual[6][i].fitness, chosen_individual[7][i].fitness, chosen_individual[8][i].fitness);
  }

  /* Printing */
  print_func(chosen_individual);

  return 0;
}
/*'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/

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

int find_number_of_teachers(){                                          
  FILE *teacher_file = fopen("info.txt", "r");
  int i = 0;    
  char str[200];
  if(teacher_file == NULL){
    perror("Error the file is empty");
    fclose(teacher_file);
    return -1; 
  }
  /* Keeps reading data into str from teacherinfo, until end of file is reached
     Each line represent a single teacher, so everytime a line is read, i is
     being counted up by one */
  while(!feof(teacher_file)){
    fgets(str, DEFAULT_LENGTH_STRING, teacher_file); 
    i++;
  }
  fclose(teacher_file);
  return i; 
}

void read_teachers_name(teacher teacher_data[][NUMBER_OF_SUBJECTS], int number_teachers){
 
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
      " %s "
      "%s "
      "%d "
      "%s ",
      local_teacher_data.teacher_name, 
      local_teacher_data.lesson_name, 
      &local_teacher_data.number_of_lessons, 
      local_teacher_data.class_name);
      printf(" %d ", local_teacher_data.number_of_lessons);
      teacher_data[j][i] = local_teacher_data; 
  
    } 
    printf("\n");
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
      result.individual_num[i][j] = rand() % NUMBER_OF_DIFFERENT_LESSONS;
    }
  }
  return result;
}

void selektion(individual individuals[][NUMBER_OF_INDIVIDUALS]){
  individual individuals_temp[NUMBER_OF_CLASSES][NUMBER_OF_INDIVIDUALS];
  int i, res;

  /* Makes individuals_temp equal to individuals */
  make_old_population(individuals, individuals_temp);

  /* Runs trough all individuals, 3 classes at a time (parralelclasses) and picks a combination of parralelclasses for each grade (7, 8 and 9) for each wanted 
     number of individuals */
  for(int j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for (i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      res = pick_individual(individuals_temp, individuals, j, i);
    }
  }
}

/* int class represents 7, 8, 9 class as: 0 = 7a, 1 = 7b, 2 = 7c, 3 = 8a, 4 = 8b and so on. Individual_Number tells which individual is currently being changed */
int pick_individual(individual temp_individuals[][NUMBER_OF_INDIVIDUALS], individual individuals[][NUMBER_OF_INDIVIDUALS], int class, int individual_number){
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
      individuals[class][individual_number] = temp_individuals[class][i];
      individuals[class+1][individual_number] = temp_individuals[class+1][i];
      individuals[class+2][individual_number] = temp_individuals[class+2][i];
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
    sum_parrallel[i] = individuals[class][i].fitness + individuals[class+1][i].fitness + individuals[class+2][i].fitness;
  }
}


/* A for loop is run as many times as the possible mutations is set. Each time the loop is run, there is a specific chance (CHANCE_OF_MUTATION) that mutations
   will ocour. When mutation occours, program picks 2 random days that are not the same and 2 random lessons that are not the same. the first random lesson 
   in the first random day is then swapped with the second random lesson in the second random day S*/
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]){
  int i = 0, j = 0, k =0, ran1Day = 0, ran1Week = 0, ran2Day = 0, ran2Week = 0, chance = 0, mutations = 0, temp = 0;
  for(k = 0; k < NUMBER_OF_CLASSES; k++){
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      chance = rand()% 100;
      mutations = rand()% MAX_MUTATIONS_PER_INDIVIDUAL;
      for (j = 0; j < mutations; j++){
        if (chance > CHANCE_OF_MUTATION){
          do {
            ran1Week = rand()% SCHOOL_DAYS_IN_WEEK;
            ran1Day = rand()% LESSONS_PER_DAY_MAX;
            ran2Week = rand()% SCHOOL_DAYS_IN_WEEK;
            ran2Day = rand()% LESSONS_PER_DAY_MAX;
          } while ((ran1Week == ran2Week) && (ran1Day == ran2Day));
        
          temp = individuals[j][i].individual_num[ran1Day][ran1Week];
          individuals[j][i].individual_num[ran1Day][ran1Week] = individuals[j][i].individual_num[ran2Day][ran2Week];
          individuals[j][i].individual_num[ran2Day][ran2Week] = temp;
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
void calculate_fitness_one(individual *individual_master, individual *individual_parallel1, individual *individual_parallel2, int h_classes[], teacher teacher_data[][NUMBER_OF_SUBJECTS], int class_master, int class_other1, int class_other2, requirements requirements_class){
  /* Reset the fitness */
  individual_master->fitness = 1;
  int i, j, k;

  /* Lessons in row (breaks counts as a lesson)*/
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i += 2){
      if (individual_master->individual_num[i][j] == individual_master->individual_num[i+1][j]){
        individual_master->fitness += FITNESS_LESSONS_IN_ROW;
      }
    }
  }
  
  /* Parallel classes - lessons in a sync */
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      if (individual_master->individual_num[i][j] == individual_parallel1->individual_num[i][j]){
        individual_master->fitness += FITNESS_PARALEL_CLASS;
      }
      if (individual_master->individual_num[i][j] == individual_parallel2->individual_num[i][j]){
        individual_master->fitness += FITNESS_PARALEL_CLASS;
      }
    }
  }
  
  /* Heavy lessons after 11.20 is bad  */
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSON_OVER_MIDDAY; i++){
      for (k = 0; k < NUMBER_OF_HEAVY_LESSONS; k++){
        if (individual_master->individual_num[i][j] == h_classes[k]){
          individual_master->fitness += FITNESS_HEAVY_LESSONS;
        }
      }
    }
  }

  /* Free lessons */
  int count_free_lessons = 0;
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      /* Need to be a free lesson - count op */
      if (individual_master->individual_num[i][j] == fri){
        count_free_lessons++;
      }

      /* In the middle */
      if (i < 6){
        if (individual_master->individual_num[i][j] == fri){
          individual_master->fitness += FITNESS_FREE_IN_MIDLE;
          
        }
      }
      if (i == 6){
        if ((individual_master->individual_num[i][j] == fri) && (individual_master->individual_num[i+1][j] != fri)){
          individual_master->fitness += FITNESS_FREE_IN_MIDLE;
        }
      }
      
      /* In the end */
      if ((individual_master->individual_num[i][j] == fri) && (i == 7) ) {
        individual_master->fitness += FITNESS_BONUS_FREE_END;
        if (individual_master->individual_num[i-1][j] == fri){
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
      if (strcmp(teacher_data[class_master][(individual_master->individual_num[i][j])].teacher_name, temp_teacher_name) != 0){
        count++;
      }
      else {
        memset(temp_teacher_name, '\0', sizeof(temp_teacher_name));
        strcat(temp_teacher_name, teacher_data[class_master][individual_master->individual_num[i][j]].teacher_name);
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
      if (test == individual_master->individual_num[i][j]){
        count++;
      }
      else {
        test = individual_master->individual_num[i][j];
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
      switch (individual_master->individual_num[i][j]){
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
        case fri:
          temp_Fri_req--;
          break;
        case rel:
          temp_Rel_req--;
          break;
        case pra:
          temp_Pra_req--;
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

  if (temp_Fri_req > 0){
    individual_master->fitness += FITNESS_NOT_MEET_REQ;
  }
  else if (temp_Fri_req < TOO_MANY_OF_LESSON){
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
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      if (strcmp(teacher_data[class_master][individual_master->individual_num[i][j]].teacher_name, teacher_data[class_other1][individual_parallel1->individual_num[i][j]].teacher_name) == 0){
        individual_master->fitness += FITNESS_TEACHER_OVERBOOKED;
      }
      if (strcmp(teacher_data[class_master][individual_master->individual_num[i][j]].teacher_name, teacher_data[class_other2][individual_parallel2->individual_num[i][j]].teacher_name) == 0){
        individual_master->fitness += FITNESS_TEACHER_OVERBOOKED;
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
  requirements requirements_temp;


  requirements_temp.Dan_req = 0;
  requirements_temp.Mat_req = 0;
  requirements_temp.Eng_req = 0;
  requirements_temp.Tys_req = 0;
  requirements_temp.Fys_req = 0;
  requirements_temp.His_req = 0;
  requirements_temp.Sam_req = 0;
  requirements_temp.Bio_req = 0;
  requirements_temp.Geo_req = 0;
  requirements_temp.Rel_req = 0;
  requirements_temp.His_req = 0;
  requirements_temp.Val_req = 0;
  requirements_temp.Pra_req = 0;
  requirements_temp.Fri_req = 0;
  requirements_temp.Gym_req = 0;


  for(k = 0; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].individual_num[k][0] = fri;
    individuals[class][indi_num].individual_num[k][1] = fri;
    individuals[class][indi_num].individual_num[k][2] = fri;
    individuals[class][indi_num].individual_num[k][3] = fri;
    individuals[class][indi_num].individual_num[k][4] = fri;
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
    individuals[class][indi_num].individual_num[k][0] = parent1.individual_num[k][ran1day];
    individuals[class][indi_num].individual_num[k][1] = parent2.individual_num[k][ran2day];
  } 

  ran_lesson = rand()% LESSONS_PER_DAY_MAX;
  for(k = 0; k < ran_lesson; k++){   
    individuals[class][indi_num].individual_num[k][2] = parent2.individual_num[k][ran6day];
    individuals[class][indi_num].individual_num[k][3] = parent1.individual_num[k][ran5day];
  }
  for(k = ran_lesson; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].individual_num[k][2] = parent1.individual_num[k][ran3day];
    individuals[class][indi_num].individual_num[k][3] = parent2.individual_num[k][ran4day];
  }
  
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK-1; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      switch (individuals[class][indi_num].individual_num[j][i]){
        case dan:
          requirements_temp.Dan_req++;
          break;
        case mat:
          requirements_temp.Mat_req++;
          break;
        case eng:
          requirements_temp.Eng_req++;
          break;
        case tys:
          requirements_temp.Tys_req++;
          break;
        case fys:
          requirements_temp.Fys_req++;
          break;
        case his:
          requirements_temp.His_req++;
          break;
        case sam:
          requirements_temp.Sam_req++;
          break;
        case val:
          requirements_temp.Val_req++;
          break;
        case geo:
          requirements_temp.Geo_req++;
          break;
        case bio:
          requirements_temp.Bio_req++;
          break;
        case fri:
          requirements_temp.Fri_req++;
          break;
        case gym:
          requirements_temp.Gym_req++;
          break;
        case rel:
          requirements_temp.Rel_req++;
          break;
        case pra:
          requirements_temp.Pra_req++;
          break;
        default:
          printf("ERROR IN FITNESS\n");
          break;
      }
    }
  }

  chance = 0;
  for(i = 0, k = 0; i < 200; i++){
    chance = rand()% NUMBER_OF_SUBJECTS;
    switch(chance){
      case dan:
        if(requirements_temp.Dan_req < requirements_classes.Dan_req){
          individuals[class][indi_num].individual_num[k][4] = dan;
          requirements_temp.Dan_req++;
          k++;
        }
        break;
      case mat:
        if(requirements_temp.Mat_req < requirements_classes.Mat_req){
          individuals[class][indi_num].individual_num[k][4] = mat;
          requirements_temp.Mat_req++;
          k++;
        }
        break;
      case eng:
        if(requirements_temp.Eng_req < requirements_classes.Eng_req){
          individuals[class][indi_num].individual_num[k][4] = eng;
          requirements_temp.Eng_req++;
          k++;
        }
       break;
      case tys:
        if(requirements_temp.Tys_req < requirements_classes.Tys_req){
          individuals[class][indi_num].individual_num[k][4] = tys;
          requirements_temp.Tys_req++;
          k++;
        }           
        break;
      case fys:
        if(requirements_temp.Fys_req < requirements_classes.Fys_req){
          individuals[class][indi_num].individual_num[k][4] = fys;
          requirements_temp.Fys_req++;
          k++;
        }
       break;
      case his:
        if(requirements_temp.His_req < requirements_classes.His_req){
          individuals[class][indi_num].individual_num[k][4] = his;
          requirements_temp.His_req++;
          k++;
        }
        break;
      case sam:
        if(requirements_temp.Sam_req < requirements_classes.Sam_req){
          individuals[class][indi_num].individual_num[k][4] = sam;
          requirements_temp.Sam_req++;
          k++;
        }
        break;
      case val:
        if(requirements_temp.Val_req < requirements_classes.Val_req){
          individuals[class][indi_num].individual_num[k][4] = val;
          requirements_temp.Val_req++;
          k++;
        }
        break;
      case geo:
        if(requirements_temp.Geo_req < requirements_classes.Geo_req){
          individuals[class][indi_num].individual_num[k][4] = geo;
          requirements_temp.Geo_req++;
          k++;
        }
        break;
      case bio:
        if(requirements_temp.Bio_req < requirements_classes.Bio_req){
          individuals[class][indi_num].individual_num[k][4] = bio;
          requirements_temp.Bio_req++;
          k++;
        }
        break;
      case gym:
        if(requirements_temp.Gym_req < requirements_classes.Gym_req){
          individuals[class][indi_num].individual_num[k][4] = gym;
          requirements_temp.Gym_req++;
          k++;
        }
        break;
      case rel:
        if(requirements_temp.Rel_req < requirements_classes.Rel_req){
          individuals[class][indi_num].individual_num[k][4] = rel;
          requirements_temp.Rel_req++;
          k++;
        }
        break;
      case pra:
        if(requirements_temp.Pra_req < requirements_classes.Pra_req){
          individuals[class][indi_num].individual_num[k][4] = pra;
          requirements_temp.Pra_req++;
          k++;
        }
        break;
      default:
        break;
    }
  }
}

void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]){
  int j = 0, i = 0;  
  for(j = 0; j < NUMBER_OF_CLASSES; j++){ 
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      individuals_temp[j][i] = individuals[j][i];
    }
  }
}

void print_func(individual chosen_individual[][NUMBER_OF_GENERATIONS]){
  int i, j, c;
  for (c = 0; c < NUMBER_OF_CLASSES; c++){
    /* Printing the days */
    printf("  Class: ");
    print_class_name(c);
    printf("  Has a fitness of: %d   The perfection grade is: %d \n", chosen_individual[c][NUMBER_OF_GENERATIONS-1].fitness, chosen_individual[c][NUMBER_OF_GENERATIONS].perfection);
    printf("  Tidspunkt\t\tMandag\t\tTirsdag\t\tOnsdag\t\tTorsdag\t\tFredag\n");
    printf("  ------------------------------------------------------------------------------------------------\n");
    
    for (i = 0; i < LESSONS_PER_DAY_MAX; i++){
      /* Printing the times */
      print_time_func(i);

      for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
        /* Printing lesson name */
        print_lesson_name(chosen_individual[c][NUMBER_OF_GENERATIONS-1].individual_num[j][i]);
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

void print_lesson_name(int num){
  switch (num){
    case dan:
      printf("Dan");
      break;
    case mat:
      printf("Mat");
      break;
    case eng:
      printf("Eng");
      break;
    case tys:
      printf("Tys");
      break;
    case fys:
      printf("Fys");
      break;
    case his:
      printf("His");
      break;
    case sam:
      printf("Sam");
      break;
    case val:
      printf("Val");
      break;
    case geo:
      printf("Geo");
      break;
    case bio:
      printf("Bio");
      break;
    case gym:
      printf("Gym");
      break;
    case fri:
      printf("---");
      break;
    case rel:
      printf("Rel");
      break;
    case pra:
      printf("Pra");
      break;
    default:
      printf("ERROR\n");
      break;
  }
}
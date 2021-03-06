#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define LESSON_NAME_MAX 12
#define SCHOOL_DAYS_IN_WEEK 5
#define TEACHER_NAME_MAX 4
#define LESSONS_PER_DAY_MAX 8
#define LESSONS_PER_WEEK_MAX (LESSONS_PER_DAY_MAX * SCHOOL_DAYS_IN_WEEK)
#define NUMBER_OF_DIFFERENT_LESSONS 13
#define DEFAULT_LENGTH_STRING 200
#define LESSON_OVER_MIDDAY 4
#define NUMBER_OF_HEAVY_LESSONS 5
#define NUMBER_OF_CLASSES 9
#define NUMBER_OF_FAG 13

#define MAX_LESSONS_IN_ROW 3

#define NUMBER_OF_INDIVIDUALS 100
#define NUMBER_OF_GENERATIONS 50

#define CHANCE_OF_MUTATION 20
#define MAX_MUTATIONS_PER_INDIVIDUAL 5

#define FITNESS_LESSONS_IN_ROW 2
#define FITNESS_PARALEL_CLASS 5
#define FITNESS_HEAVY_LESSONS -2
#define FITNESS_FREE_IN_MIDLE -100
#define FITNESS_MANY_LESSONS_IN_ROW -5

enum lesson_number {dan, mat, eng, tys, fys, his, sam, valg, geo, bio, gym, fri, rel, pra};

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
void read_teachers_name(teacher teacher_data[][NUMBER_OF_FAG], int number_teachers);
void find_req(teacher teacher_data[][NUMBER_OF_FAG], requirements requirements_classes[]);
void create_individuals(individual individuals[][NUMBER_OF_INDIVIDUALS]);
individual create_individual();
void selektion(individual individuals[][NUMBER_OF_INDIVIDUALS]);
int pick_individual(individual temp_individuals[][NUMBER_OF_INDIVIDUALS], individual individuals[][NUMBER_OF_INDIVIDUALS], int class, int individual_number);
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]);
void calculate_fitness_all(individual individuals[][NUMBER_OF_INDIVIDUALS], int h_classes[]);
void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int h_classes[]);
void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]);
void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]);
void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], int class, int parent_num, int indi_num);
void choose_individual(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_chosen[][NUMBER_OF_GENERATIONS], int class, int generation);
void calculate_finess_parallel(individual individuals[][NUMBER_OF_INDIVIDUALS], int sum_parrallel[], int class);
void create_schedule(lesson week[][SCHOOL_DAYS_IN_WEEK * LESSONS_PER_DAY_MAX], individual indi, int class);
lesson create_lesson(int num);
void print_schedule(lesson week[][SCHOOL_DAYS_IN_WEEK * LESSONS_PER_DAY_MAX], int class);
void print_lesson(lesson l);

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
  teacher teacher_data[NUMBER_OF_CLASSES][NUMBER_OF_FAG];
  read_teachers_name(teacher_data, number_of_teacher);
  find_req(teacher_data, requirements_classes);
  /* Init genetic algorithem */
  srand(time(NULL));

  create_individuals(individuals);
  calculate_fitness_all(individuals, h_classes);

  /* Genetic algorithem */
  int i, j;
  for (i = 0; i < NUMBER_OF_GENERATIONS; i++){
    /* Fitness */
    calculate_fitness_all(individuals, h_classes);

    /* Selektion */
    selektion(individuals);

    /* Mutation */
    mutation(individuals);

    /* Crossover */
    crossover(individuals, individuals_temp);

     /* Tests */
    
    for(j = 0; j < (NUMBER_OF_CLASSES); j += 3){
      choose_individual(individuals, chosen_individual, j, i);  
    }
    printf("%d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \n", chosen_individual[0][i].fitness, chosen_individual[1][i].fitness, chosen_individual[2][i].fitness, chosen_individual[3][i].fitness, chosen_individual[4][i].fitness, chosen_individual[5][i].fitness, chosen_individual[6][i].fitness, chosen_individual[7][i].fitness, chosen_individual[8][i].fitness);
  }

  /* Printing */
  for(i = 0; i < NUMBER_OF_CLASSES; i++){
    printf("  The fitness is: %d   8.A's Skoleschedule: \n", chosen_individual[i][48].fitness);
    create_schedule(week, chosen_individual[i][48], i);
    print_schedule(week, i);

    if (i % 3 == 0){
      printf("\n\n");
    }
  }

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
  FILE *teacher_file = fopen("teacherinfo.txt", "r");
  int i = 0;    
  char str[200];
  if(teacher_file == NULL){
    perror("Error the file is empty");
    fclose(teacher_file);
    exit(1);
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

void read_teachers_name(teacher teacher_data[][NUMBER_OF_FAG], int number_teachers){
 
 FILE *teacherinfo = fopen("teacherinfo.txt", "r");
  if(teacherinfo == NULL){
  perror("Error the file is empty");
  fclose(teacherinfo);
  exit(1);
  }
  teacher local_teacher_data;
  int i = 0, j = 0;
  for(j = 0; j < NUMBER_OF_CLASSES; j++){
    for(i = 0;i < NUMBER_OF_FAG; i++){
      fscanf(teacherinfo,
      " %s "
      "%s "
      " %d "
      "%s ",
      local_teacher_data.teacher_name, 
      local_teacher_data.lesson_name, 
      &local_teacher_data.number_of_lessons, 
      local_teacher_data.class_name);

      teacher_data[j][i] = local_teacher_data; 
  
    } 
  }  
  fclose(teacherinfo);
}

void find_req(teacher teacher_data[][NUMBER_OF_FAG], requirements requirements_classes[]){
  int j = 0, i  = 0;
  for(j = 0; j < NUMBER_OF_CLASSES; j++){
    for(i = 0; i < NUMBER_OF_FAG; i ++){
      if(strcmp("Dan", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Dan_req = teacher_data[j][i].number_of_lessons;
        printf("%d \n ", requirements_classes[j].Dan_req);
      }
      else if(strcmp("Mat", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Mat_req = teacher_data[j][i].number_of_lessons;
      }
      else if(strcmp("Eng", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Eng_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Tys", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Tys_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Fys", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Fys_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("His", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].His_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Val", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Val_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Geo", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Geo_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Bio", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Bio_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Gym", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Gym_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Rel", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Rel_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Fri", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Fri_req = teacher_data[j][i].number_of_lessons;  
      }
      else if(strcmp("Pra", teacher_data[j][i].lesson_name) == 0){
        requirements_classes[j].Pra_req = teacher_data[j][i].number_of_lessons;  
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

  /* temp == individual */
  make_old_population(individuals, individuals_temp);
  for(int j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for (i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      res = pick_individual(individuals_temp, individuals, j, i);
      assert(res == 1);
    }
  }
}

int pick_individual(individual temp_individuals[][NUMBER_OF_INDIVIDUALS], individual individuals[][NUMBER_OF_INDIVIDUALS], int class, int individual_number){
  int i, sum = 0, j;
  int fitness_test = 0;
  int sum_parrallel[NUMBER_OF_INDIVIDUALS];

  /* First the fitness from all individuels is added together to make the roulette */

  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    sum_parrallel[i] = temp_individuals[class][i].fitness + temp_individuals[class+1][i].fitness + temp_individuals[class+2][i].fitness;
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

void choose_individual(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_chosen[][NUMBER_OF_GENERATIONS], int class, int generation){
  int choose_fitness = -100000;
  int sum_parrallel_fitness[NUMBER_OF_INDIVIDUALS];
  int i, j, k, f;
  
  calculate_finess_parallel(individuals, sum_parrallel_fitness, class);

    for(k = 0; k < NUMBER_OF_INDIVIDUALS; k++){
      if (sum_parrallel_fitness[k] > choose_fitness){
        individuals_chosen[class][generation] = individuals[class][k];
        individuals_chosen[class+1][generation] = individuals[class+1][k];
        individuals_chosen[class+2][generation] = individuals[class+2][k];
        choose_fitness = sum_parrallel_fitness[k];
      }
    } 
}

void calculate_finess_parallel(individual individuals[][NUMBER_OF_INDIVIDUALS], int sum_parrallel[], int class){
  int i = 0;
  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    sum_parrallel[i] = individuals[class][i].fitness + individuals[class+1][i].fitness + individuals[class+2][i].fitness;
  }
}

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

void calculate_fitness_all(individual individuals[][NUMBER_OF_INDIVIDUALS], int h_classes[]){
  int i, j;
  for(j = 0; j < NUMBER_OF_CLASSES; j+=3){
    for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
      calculate_fitness_one(&(individuals[j][i]), &(individuals[j+2][i]), &(individuals[j+1][i]), h_classes);
      calculate_fitness_one(&(individuals[j+1][i]), &(individuals[j][i]), &(individuals[j][i+2]), h_classes);
      calculate_fitness_one(&(individuals[j+2][i]), &(individuals[j+1][i]), &(individuals[j][i]), h_classes);
    }
  }  
}

void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int h_classes[]){
  /* Reset the fitness */
  individual_master->fitness = 0;

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
      if (individual_master->individual_num[i][j] == individual_other1->individual_num[i][j]){
        individual_master->fitness += FITNESS_PARALEL_CLASS;
      }
      if (individual_master->individual_num[i][j] == individual_other2->individual_num[i][j]){
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

  /* Free lessons - in the middle */
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
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
    }
  }

  /* forberedsestimer */


  /* Too many of the same in a row */
  int count = 0, test = 0;
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

  /* If it is negative, change to zero */
  if (individual_master->fitness < 0){
    individual_master->fitness = 0;
  }
}

void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]){
  int i = 0, ran1day = 0, ran2day = 0, ran3day = 0, ran4day = 0, ran5day = 0, ran6day = 0, ran_lesson = 0, k = 0, j = 0;
  individual parent1;
  individual parent2;

  make_old_population(individuals, individuals_temp);
  for(j = 0; j < NUMBER_OF_CLASSES; j += 3){
    for(i = 0, k = 0; k < NUMBER_OF_INDIVIDUALS; i += 2, k++){
      if(i > 199){
        i = 0;
      }
      crossover_indi(individuals, individuals_temp, j, i, k);
      crossover_indi(individuals, individuals_temp, j+1, i, k);
      crossover_indi(individuals, individuals_temp, j+2, i, k);
    }
  }
} 

void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], int class, int parent_num, int indi_num){
  int i = 0, ran1day = 0, ran2day = 0, ran3day = 0, ran4day = 0, ran5day = 0, ran6day = 0, ran_lesson = 0, k = 0, j = 0;
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
    individuals[class][indi_num].individual_num[k][0] = individuals_temp[class][parent_num].individual_num[k][ran1day];
    individuals[class][indi_num].individual_num[k][1] = individuals_temp[class][parent_num+1].individual_num[k][ran2day];
    individuals[class][indi_num].individual_num[k][4] = individuals_temp[class][parent_num+1].individual_num[k][ran2day]; 
  } 
  ran_lesson = rand()% LESSONS_PER_DAY_MAX;
  for(k = 0; k < ran_lesson; k++){   
    individuals[class][indi_num].individual_num[k][2] = individuals_temp[class][parent_num+1].individual_num[k][ran6day];
    individuals[class][indi_num].individual_num[k][3] = individuals_temp[class][parent_num].individual_num[k][ran5day];
  }
  for(k = LESSONS_PER_DAY_MAX - ran_lesson; k < LESSONS_PER_DAY_MAX; k++){   
    individuals[class][indi_num].individual_num[k][2] = individuals_temp[class][parent_num].individual_num[k][ran3day];
    individuals[class][indi_num].individual_num[k][3] = individuals_temp[class][parent_num+1].individual_num[k][ran4day];
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

void create_schedule(lesson week[][SCHOOL_DAYS_IN_WEEK * LESSONS_PER_DAY_MAX], individual indi, int class){
  int lesson_now = 0, j = 0, k = 0;
  /* Creating the schedule based on the individual */
  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(int i = 0; i < LESSONS_PER_DAY_MAX; i++){
      week[class][k] = create_lesson(indi.individual_num[i][j]);
      k++;
    }
  }
}

lesson create_lesson(int num){
  lesson result;
  
  /* Making the lessons name */
  switch (num){
    case dan:
      strcpy(result.lesson_name, "Dan");
      break;
    case mat:
      strcpy(result.lesson_name, "Mat");
      break;
    case eng:
      strcpy(result.lesson_name, "Eng");
      break;
    case tys:
      strcpy(result.lesson_name, "Tysk");
      break;
    case fys:
      strcpy(result.lesson_name, "Fys");
      break;
    case his:
      strcpy(result.lesson_name, "His");
      break;
    case sam:
      strcpy(result.lesson_name, "Sam");
      break;
    case valg:
      strcpy(result.lesson_name, "Valg");
      break;
    case geo:
      strcpy(result.lesson_name, "Geo");
      break;
    case bio:
      strcpy(result.lesson_name, "Bio");
      break;
    case gym:
      strcpy(result.lesson_name, "Gym");
      break;
    case fri:
      strcpy(result.lesson_name, "---");
      break;
    case rel:
      strcpy(result.lesson_name, "Rel");
      break;
    case pra:
      strcpy(result.lesson_name, "Pra");
      break;  
    default:
      strcpy(result.lesson_name, "ERROR");
      printf("ERROR: The number is: %d\n", num);
      break;
  }

  return result;
}

void print_schedule(lesson week[][SCHOOL_DAYS_IN_WEEK * LESSONS_PER_DAY_MAX], int class){
  int lesson_of_day = 0, day_of_week = 0, lesson_in_individual = 0, done = 0, lesson_number = 0;
  printf("  Tidspunkt\t\tMandag\t\tTirsdag\t\tOnsdag\t\tTorsdag\t\tFredag\n");
  printf("  ------------------------------------------------------------------------------------------------\n");
  /* printing the first time */
  printf("   8.00 -  8.45  |\t");

  /* Printing the schedule */
  while (!done){
    /* Making the next number for the lesson */
    /* Printing the lesson and the teacher */
    print_lesson(week[class][lesson_number]);
    lesson_number++;
    day_of_week++;

    /* Making new line */
    if (day_of_week == SCHOOL_DAYS_IN_WEEK){
      day_of_week = 0;
      lesson_of_day++;
      /* Making an ekstra new line to act as a break ind school */
      if ((lesson_of_day % 2) == 0){
        printf("\n");
      }
      printf("\n  ");

      if(lesson_of_day == 1){
        printf(" 8.45 -  9.30  |\t");
      }
      else if(lesson_of_day == 2){
        printf(" 9.50 - 10.35  |\t");
      }
      else if(lesson_of_day == 3){
        printf("10.35 - 11.20  |\t");
      }
      else if(lesson_of_day == 4){
        printf("11.50 - 12.35  |\t");
      }
      else if(lesson_of_day == 5){
        printf("12.35 - 13.20  |\t");
      }
      else if(lesson_of_day == 6){
        printf("13.30 - 14.15  |\t");
      }
      else if(lesson_of_day == 7){
        printf("14.15 - 15.00  |\t");
      }
    }

    /* If all the lessons has printed, then break the while */
    if (lesson_of_day == LESSONS_PER_DAY_MAX){
      done = 1;
    }
  }
}

void print_lesson(lesson l){
  printf("%s",l.lesson_name);
  for(int i = 0; i < LESSON_NAME_MAX-strlen(l.lesson_name); i++){
    printf(" ");
  }
}
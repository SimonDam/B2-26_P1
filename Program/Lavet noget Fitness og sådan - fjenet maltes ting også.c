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
#define NUMBER_OF_INDIVIDUALS 100
#define DEFAULT_LENGTH_STRING 200
#define LESSON_OVER_MIDDAY 4
#define NUMBER_OF_HEAVY_LESSONS 4

#define FITNESS_LESSONS_IN_ROW 1
#define FITNESS_PARALEL_CLASS 5
#define FITNESS_HEAVY_LESSONS -50
#define FITNESS_FREE_IN_MIDLE -1000

enum lesson_number {dan, mat, eng, tys, fys, his, sam, valg, geo, bio, gym, fri, rel, hda};

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
};

int find_number_of_teachers();
void make_teachers_names(FILE *teachers, char teachers_names[]);
void create_individuals(individual individuals[]);
individual create_individual();
individual choose_individual(individual individuals[]);
void calculate_fitness_all(individual individuals_a[], individual individuals_b[], individual individuals_c[], int H_fag[]);
void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int H_fag[]);
individual choose_individual(individual individuals[]);
void create_skema(lesson week[][SCHOOL_DAYS_IN_WEEK], individual indi, char teachers_names[]);
lesson create_lesson(int num, char teachers_names[]);
void create_skema(lesson week[][SCHOOL_DAYS_IN_WEEK], individual indi, char teachers_names[]);
lesson create_lesson(int num, char teachers_names[]);
void print_skema(lesson week[][SCHOOL_DAYS_IN_WEEK]);
void print_lesson_teacher(lesson l);
void print_lesson(lesson l);

individual merge_individuals(individual individualA, individual individualB);
void insert_new_days(individual * dest_individual, individual deliver_individual, int day1, int day2);
void complete_missing_day(individual * incomplete_individual);

/***************************MAIN******************************/
int main(void){
  /* read teachers */
  FILE *teachers = fopen("teachers.txt", "r");
  char teachers_names[TEACHER_NAME_MAX * NUMBER_OF_DIFFERENT_LESSONS];
  memset(teachers_names, '\0', TEACHER_NAME_MAX * NUMBER_OF_DIFFERENT_LESSONS);
  make_teachers_names(teachers, teachers_names);
  fclose(teachers);

  /* init general stuff */
  int H_fag[NUMBER_OF_HEAVY_LESSONS] = {mat, fys, eng, dan};

  /* init 9 */
  lesson week9a[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual9a;
  individual individuals9a[NUMBER_OF_INDIVIDUALS];
  
  lesson week9b[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual9b;
  individual individuals9b[NUMBER_OF_INDIVIDUALS];

  lesson week9c[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual9c;
  individual individuals9c[NUMBER_OF_INDIVIDUALS];

  /* init 8 */
  lesson week8a[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual8a;
  individual individuals8a[NUMBER_OF_INDIVIDUALS];

  lesson week8b[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual8b;
  individual individuals8b[NUMBER_OF_INDIVIDUALS];

  lesson week8c[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual8c;
  individual individuals8c[NUMBER_OF_INDIVIDUALS];

  /* init 7 */
  lesson week7a[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual7a;
  individual individuals7a[NUMBER_OF_INDIVIDUALS];

  lesson week7b[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual7b;
  individual individuals7b[NUMBER_OF_INDIVIDUALS];

  lesson week7c[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual7c;
  individual individuals7c[NUMBER_OF_INDIVIDUALS];


  /* Init genetic algorithem */
  srand(time(NULL));

  create_individuals(individuals9a);
  create_individuals(individuals9b);
  create_individuals(individuals9c);
  calculate_fitness_all(individuals9a, individuals9b, individuals9c, H_fag);
  
  create_individuals(individuals8a);
  create_individuals(individuals8b);
  create_individuals(individuals8c);
  calculate_fitness_all(individuals8a, individuals8b, individuals8c, H_fag);
  
  create_individuals(individuals7a);
  create_individuals(individuals7b);
  create_individuals(individuals7c);
  calculate_fitness_all(individuals7a, individuals7b, individuals7c, H_fag);


  /* Genetic algorithem */

  chosen_individual9a = choose_individual(individuals9a);
  chosen_individual9b = choose_individual(individuals9b);
  chosen_individual9c = choose_individual(individuals9c);

  chosen_individual8a = choose_individual(individuals8a);
  chosen_individual8b = choose_individual(individuals8b);
  chosen_individual8c = choose_individual(individuals8c);

  chosen_individual7a = choose_individual(individuals7a);
  chosen_individual7b = choose_individual(individuals7b);
  chosen_individual7c = choose_individual(individuals7c);


  /* Printing */
  printf("  The fitness is: %d   9.A's Skoleskema: \n", chosen_individual9a.fitness);
  create_skema(week9a, chosen_individual9a, teachers_names);
  print_skema(week9a);
  printf("  The fitness is: %d   9.B's Skoleskema: \n", chosen_individual9b.fitness);
  create_skema(week9b, chosen_individual9b, teachers_names);
  print_skema(week9b);
  printf("  The fitness is: %d   9.C's Skoleskema: \n", chosen_individual9c.fitness);
  create_skema(week9c, chosen_individual9c, teachers_names);
  print_skema(week9c);
  printf("\n\n");

  printf("  The fitness is: %d   8.A's Skoleskema: \n", chosen_individual8a.fitness);
  create_skema(week8a, chosen_individual8a, teachers_names);
  print_skema(week8a);
  printf("  The fitness is: %d   8.B's Skoleskema: \n", chosen_individual8b.fitness);
  create_skema(week8b, chosen_individual8b, teachers_names);
  print_skema(week8b);
  printf("  The fitness is: %d   8.C's Skoleskema: \n", chosen_individual8c.fitness);
  create_skema(week8c, chosen_individual8c, teachers_names);
  print_skema(week8c);
  printf("\n\n");

  printf("  The fitness is: %d   7.A's Skoleskema: \n", chosen_individual7a.fitness);
  create_skema(week7a, chosen_individual7a, teachers_names);
  print_skema(week7a);
  printf("  The fitness is: %d   7.B's Skoleskema: \n", chosen_individual7b.fitness);
  create_skema(week7b, chosen_individual7b, teachers_names);
  print_skema(week7b);
  printf("  The fitness is: %d   7.C's Skoleskema: \n", chosen_individual7c.fitness);
  create_skema(week7c, chosen_individual7c, teachers_names);
  print_skema(week7c);
  printf("\n\n");

  return 0;
}
/**************************************************************/

int find_number_of_teachers(){                                          
  FILE *teacher_file = fopen("teacherinfo.txt", "r");
  int i = 0;    
  char str[200];
  if(teacher_file == NULL){
    perror("Error the file is empty");
    fclose(teacher_file);
    return -1; 
  }
  while(!feof(teacher_file)){
    fgets(str, DEFAULT_LENGTH_STRING, teacher_file); 
    i++;
  }
  fclose(teacher_file);
  return i; 
}

void make_teachers_names(FILE *teachers, char teachers_names[]){
  char temp_name[TEACHER_NAME_MAX];
  char temp_lesson[5];
  int res = 0, done = 0;

  while (!done){
    res += fscanf(teachers, " %s %s ",temp_lesson, temp_name);  
    
    /* Doing so it is always three long */
    if (strlen(temp_name) > 3){
      printf("ERROR: A name is longer then three");
    }
    while (strlen(temp_name) < 3){
      strcat(temp_name, " ");
    }

    /* Looking for when to stop */
    if (strcmp(temp_name, "END") == 0){
      done = 1;
    }
    /* Creating the list of names */
    else {
      strcat(teachers_names, temp_name);
    }
  }
  if (res != 28){
    printf("Error occurred reading file\n\n");
  }
}

void create_individuals(individual individuals[]){
  for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    individuals[i] = create_individual();
  }
}

individual create_individual(){
  individual result;
  /* Creating the individual */
  /* Makring it random */
  int i, j;
  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      result.individual_num[i][j] = rand() % NUMBER_OF_DIFFERENT_LESSONS;
    }
  }
  
  return result;
}

void calculate_fitness_all(individual individuals_a[], individual individuals_b[], individual individuals_c[], int H_fag[]){
  for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    printf("%d: ", i);
    calculate_fitness_one(&(individuals_a[i]), &(individuals_b[i]), &(individuals_c[i]), H_fag);
    calculate_fitness_one(&(individuals_b[i]), &(individuals_a[i]), &(individuals_c[i]), H_fag);
    calculate_fitness_one(&(individuals_c[i]), &(individuals_a[i]), &(individuals_b[i]), H_fag);
    printf("\n");
  }
  printf("\n\n");
}

void calculate_fitness_one(individual *individual_master, individual *individual_other1, individual *individual_other2, int H_fag[]){
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

  /* Parallel klasser - ens timer samme tid */
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

  /* Tunge fag over middag  */
  for (j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSON_OVER_MIDDAY; i++){
      for (k = 0; k < NUMBER_OF_HEAVY_LESSONS; k++){
        if (individual_master->individual_num[i][j] == H_fag[k]){
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

  /* Printing fitness - debugging */
  printf("  \t Fit: %d", individual_master->fitness);
}

individual choose_individual(individual individuals[]){
  individual chosen;
  chosen.fitness = -100000;

  int i, j, k;
  for (k = 0; k < NUMBER_OF_INDIVIDUALS; k++){
    if (individuals[k].fitness > chosen.fitness){
      for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
        for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
          chosen.individual_num[i][j] = individuals[k].individual_num[i][j];
          chosen.fitness = individuals[k].fitness;
        }
      }
    }
  }

  return chosen;
}

void create_skema(lesson week[][SCHOOL_DAYS_IN_WEEK], individual indi, char teachers_names[]){
  int lesson_now = 0;
  /* Creating the skema based on the individual */
  for(int j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(int i = 0; i < LESSONS_PER_DAY_MAX; i++){
      week[i][j] = create_lesson(indi.individual_num[i][j], teachers_names);
    }
  }
}

lesson create_lesson(int num, char teachers_names[]){
  lesson result;
  char temp_name[TEACHER_NAME_MAX];
  memset(temp_name, '\0', TEACHER_NAME_MAX);
  int num2 = 0;

  memset(result.teacher_name, '\0', TEACHER_NAME_MAX);
  
  for (int i = 0; i < TEACHER_NAME_MAX -1; i++){
    num2 = (num*(TEACHER_NAME_MAX-1))+i;
    temp_name[i] = teachers_names[num2];
  }
  strcat(result.teacher_name, temp_name);
  
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
    default:
      strcpy(result.lesson_name, "ERROR");
      printf("ERROR: The number is: %d\n", num);
      break;
  }

  return result;
}

void print_skema(lesson week[][SCHOOL_DAYS_IN_WEEK]){
  int lesson_of_day = 0, day_of_week = 0, lesson_in_individual = 0, done = 0;
  printf("  Tidspunkt\t\tMandag\t\tTirsdag\t\tOnsdag\t\tTorsdag\t\tFredag\n");
  printf("  ------------------------------------------------------------------------------------------------\n");
  /* printing the first time */
  printf("   8.00 -  8.45  |\t");

  /* Printing the skema */
  while (!done){
    /* Making the next number for the lesson */
    /* Printing the lesson and the teacher */
    print_lesson_teacher(week[lesson_of_day][day_of_week]);
    print_lesson(week[lesson_of_day][day_of_week]);

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

/* KOMMENTAR TIL OS SELV */
/* hvis vi går ud fra at størrelsen på lærernavn er standard, kan vi lave printf som */
/* KOMMENTAR TIL OS SELV */
/* printf("%-4s",l.teacher_name); og så er for loopet helt unødvendigt. Det samme */
/* gælder i funktionen nedenunder - Malthe */

void print_lesson_teacher(lesson l){
  printf("%s",l.teacher_name);
  for(int i = 0; i < TEACHER_NAME_MAX - strlen(l.teacher_name); i++){
    printf(" ");
  }                                                                            
}

void print_lesson(lesson l){
  printf("%s",l.lesson_name);
  for(int i = 0; i < LESSON_NAME_MAX-strlen(l.lesson_name); i++){
    printf(" ");
  }
}

/*
  Et rigtigt skoleskema fra 9. - 36timer i alt om ugen
  7 dan     - 3*2 er sammen 1 for sig selv
  5 mat     - 1*2 er sammen 3 for sig selv
  4 eng     - 2*2 er sammen
  4 tysk    - ingen sammenhængende
  3 fys     - 1*2 er sammen 1 for sig selv
  2 his     - ingen sammenhængende
  2 sam     - ingen sammenhængende
  2 valg    - 1*2 er sammen
  2 geo     - 1*2 er sammen
  2 bio     - 1*2 er sammen
  2 gym     - 1*2 er sammen
  1 rel     - ingen sammenhængende

  printf("*------------*------------*------------*------------*------------*------------*\n");
  printf("|            |Mandag      |Tirsdag     |Onsdag      |Torsdag     |Fredag      |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("| 8.00- 8.45 |SP ENG      |SP VALG2    |SCI BIO     |AH IDR GYM  |FRI         |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("| 8.45- 9.30 |SP ENG      |SP VALG2    |SCI BIO     |AH IDR GYM  |MM HIS      |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("| 9.30- 9.50 |Pause       |Pause       |Pause       |Pause       |Pause       |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("| 9.50-10.35 |JC TYSK     |AC MAT      |RA GEO      |AC MAT      |SP DANSK    |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|10.35-11.20 |SL SAM      |AC MAT      |RA GEO      |MM HIS      |SP DANSK    |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|11.20-11.50 |Pause       |Pause       |Pause       |Pause       |Pause       |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|11.50-12.35 |SP DANSK    |RA FYS      |SP DANSK    |SP DANSK    |SL SAM      |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|12.35-13.20 |SP DANSK    |RA FYS      |AC MAT      |SP DANSK    |AC MAT      |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|13.20-13.30 |Pause       |Pause       |Pause       |Pause       |Pause       |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|13.30-14.15 |SP VALG1    |JC TYSK     |JC TYSK     |JC TYSK     |SP ENG      |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
  printf("|14.15-15.00 |SP VALG1    |FRI         |RA FYS      |MMJ REL     |SP ENG      |\n");
  printf("*------------+------------+------------+------------+------------+------------*\n");
*/
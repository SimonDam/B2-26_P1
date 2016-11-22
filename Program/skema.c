#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LESSON_NAME_MAX 12
#define SCHOOL_DAYS_IN_WEEK 5
#define TEACHER_NAME_MAX 3
#define LESSONS_PER_DAY_MAX 8
#define LESSONS_PER_WEEK_MAX (LESSONS_PER_DAY_MAX * SCHOOL_DAYS_IN_WEEK)
#define NUMBER_OF_DIFFERENT_LESSONS 13

#define LESSONS_DAN 7
#define LESSONS_MAT 5
#define LESSONS_ENG 4
#define LESSONS_TYS 4
#define LESSONS_FYS 3
#define LESSONS_HIS 2
#define LESSONS_SAM 2
#define LESSONS_VALG 2
#define LESSONS_GEO 2
#define LESSONS_BIO 2
#define LESSONS_GYM 2
#define LESSONS_FRI 2
#define LESSONS_REL 1

#define FITNESS_LESSONS_IN_ROW 10

enum lesson_number {dan, mat, eng, tys, fys, his, sam, valg, geo, bio, gym, fri, rel};

typedef struct lesson lesson;
struct lesson{
  char teacher_name[TEACHER_NAME_MAX];
  char lesson_name[LESSON_NAME_MAX];
};

void create_individual(int individual[]);
void calculate_fitness(int individual[], int *fitness);
void print_skema(lesson week[]);
void create_skema(lesson week[], int individual[]);
lesson create_lesson(int num);
void print_lesson(lesson l);
void print_lesson_teacher(lesson l);

/***************************MAIN******************************/
int main(void){
  lesson week[LESSONS_PER_WEEK_MAX];
  int individual[LESSONS_PER_WEEK_MAX];
  int fitness = 0;
  srand(time(NULL));

  create_individual(individual);
  calculate_fitness(individual, &fitness);

  printf("Fitness: %d \n\n", fitness);

  create_skema(week, individual);
  print_skema(week);

  return 0;
}
/**************************************************************/

void create_individual(int individual[]){
  /* Makring it random */
  individual[0] = 1;
  individual[1] = 1;
  individual[2] = 1;
  individual[3] = 1;
  for(int i = 4; i < LESSONS_PER_WEEK_MAX; i++){
    individual[i] = rand() % NUMBER_OF_DIFFERENT_LESSONS;
  }
}

void calculate_fitness(int individual[], int *fitness){
  *fitness = 10;
  /* Lessons in row? */
  for (int i = 0; i < LESSONS_PER_WEEK_MAX; i = i +2){
    if (individual[i] == individual[i+1]){
      *fitness = *fitness + FITNESS_LESSONS_IN_ROW;
    }
  }
}

void create_skema(lesson week[], int individual[]){
  int lesson_now = 0;
  for(int i = 0; i < LESSONS_PER_WEEK_MAX; i++){
    week[i] = create_lesson(individual[i]);
  }
}

lesson create_lesson(int num){
  lesson result;
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
      strcpy(result.lesson_name, "Fri");
      break;
    case rel:
      strcpy(result.lesson_name, "Rel");
      break;
    default:
      strcpy(result.lesson_name, "ERROR");
      printf("ERROR: The number is: %d\n", num);
      break;
  }
  strcpy(result.teacher_name, "JC");

  return result;
}

void print_skema(lesson week[]){
  int lesson_of_day = 0, day_of_week = 1, lesson_in_individual = 0, done = 0;
  printf("Mandag\t\tTirsdag\t\tOnsdag\t\tTorsdag\t\tFredag\n");
  printf("-------------------------------------------------------------------------\n");
  while (!done){
    print_lesson_teacher(week[lesson_in_individual]);
    /*printf(" %d ", lesson_in_individual);*/
    print_lesson(week[lesson_in_individual]);

    if (day_of_week == SCHOOL_DAYS_IN_WEEK){
      day_of_week = 0;
      lesson_of_day++;
      printf("\n");
    }
    if (lesson_of_day == LESSONS_PER_DAY_MAX){
      done = 1;
    }
    lesson_in_individual = (LESSONS_PER_DAY_MAX*day_of_week)+lesson_of_day;
    day_of_week++;
  }
}

void print_lesson_teacher(lesson l){
  printf("%s",l.teacher_name);
  for(int i = 0; i < TEACHER_NAME_MAX-strlen(l.teacher_name); i++){
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
  printf("|10.45-11.20 |SL SAM      |AC MAT      |RA GEO      |MM HIS      |SP DANSK    |\n");
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
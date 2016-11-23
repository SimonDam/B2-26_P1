#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LESSON_NAME_MAX 13
#define SCHOOL_DAYS_IN_WEEK 5
#define TEACHER_NAME_MAX 3
#define LESSONS_PER_DAY_MAX 8
#define LESSONS_PER_WEEK_MAX (LESSONS_PER_DAY_MAX * SCHOOL_DAYS_IN_WEEK)
#define NUMBER_OF_DIFFERENT_LESSONS 13
#define NUMBER_OF_INDIVIDUALS 10

#define FITNESS_LESSONS_IN_ROW 10

enum lesson_number {dan, mat, eng, tys, fys, his, sam, valg, geo, bio, gym, fri, rel};

typedef struct lesson lesson;
struct lesson{
  char teacher_name[TEACHER_NAME_MAX];
  char lesson_name[LESSON_NAME_MAX];
};

typedef struct individual individual;
struct individual{
  int individual_num[LESSONS_PER_WEEK_MAX];
  int fitness;
};

void create_individuals(individual individuals[]);
individual create_individual();
individual choose_individual(individual individuals[]);
void calculate_fitness_all(individual individuals[]);
void calculate_fitness_one(individual *indi);
void print_skema(lesson week[]);
void create_skema(lesson week[], individual indi);
lesson create_lesson(int num);
void print_lesson(lesson l);
void print_lesson_teacher(lesson l);

/***************************MAIN******************************/
int main(void){
  /* init 9 */
  lesson week9[LESSONS_PER_WEEK_MAX];
  individual chosen_individual9;
  individual individuals9[NUMBER_OF_INDIVIDUALS];
  
  /* init 8 */
  lesson week8[LESSONS_PER_WEEK_MAX];
  individual chosen_individual8;
  individual individuals8[NUMBER_OF_INDIVIDUALS];

  /* init 7 */
  lesson week7[LESSONS_PER_WEEK_MAX];
  individual chosen_individual7;
  individual individuals7[NUMBER_OF_INDIVIDUALS];

  /* Stuff */
  srand(time(NULL));

  create_individuals(individuals9);
  calculate_fitness_all(individuals9);
  create_individuals(individuals8);
  calculate_fitness_all(individuals8);
  create_individuals(individuals7);
  calculate_fitness_all(individuals7);

  chosen_individual9 = choose_individual(individuals9);
  chosen_individual8 = choose_individual(individuals8);
  chosen_individual7 = choose_individual(individuals7);

  create_skema(week9, chosen_individual9);
  print_skema(week9);
  printf("\n\n");
  create_skema(week8, chosen_individual8);
  print_skema(week8);
  printf("\n\n");
  create_skema(week7, chosen_individual7);
  print_skema(week7);

  return 0;
}
/**************************************************************/

void create_individuals(individual individuals[]){
  for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    individuals[i] = create_individual();
  }
}

individual create_individual(){
  individual result;
  /* Creating the individual */
  /* Makring it random */
  result.individual_num[0] = mat;
  result.individual_num[1] = mat;
  result.individual_num[2] = mat;
  result.individual_num[3] = mat;
  for(int i = 4; i < LESSONS_PER_WEEK_MAX; i++){
    result.individual_num[i] = rand() % NUMBER_OF_DIFFERENT_LESSONS;
  }

  return result;
}

void calculate_fitness_all(individual individuals[]){
  for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    calculate_fitness_one(&(individuals[i]));
  }
}

void calculate_fitness_one(individual *indi){
  /* Reset the fitness */
  indi->fitness = 0;

  /* Lessons in row (breaks counts as a lesson)*/
  for (int i = 0; i < LESSONS_PER_WEEK_MAX; i = i + 2){
    if (indi->individual_num[i] == indi->individual_num[i+1]){
      indi->fitness = indi->fitness + FITNESS_LESSONS_IN_ROW;
    }
  }
}

individual choose_individual(individual individuals[]){
  individual chosen;
  chosen.fitness = 0;

  for(int i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    if (individuals[i].fitness > chosen.fitness){
      chosen.fitness = individuals[i].fitness;
      for(int j = 0; j < LESSONS_PER_WEEK_MAX; j++){
        chosen.individual_num[j] = individuals[i].individual_num[j];
      }
    }
  }
  printf("BEST FITNESS: %d\n", chosen.fitness);
  return chosen;
}

void create_skema(lesson week[], individual indi){
  int lesson_now = 0;
  /* Creating the skema based on the individual */
  for(int i = 0; i < LESSONS_PER_WEEK_MAX; i++){
    week[i] = create_lesson(indi.individual_num[i]);
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

  /* Making lessons teacher name */
  strcpy(result.teacher_name, "JC");

  return result;
}

void print_skema(lesson week[]){
  int lesson_of_day = 0, day_of_week = 0, lesson_in_individual = 0, done = 0;
  printf("Tidspunkt\t\tMandag\t\tTirsdag\t\tOnsdag\t\tTorsdag\t\tFredag\n");
  printf("------------------------------------------------------------------------------------------------\n");
  /* printing the first time */
  printf(" 8.00 -  8.45  |\t");

  /* Printing the skema */
  while (!done){
    /* Making the next number for the lesson */
    lesson_in_individual = (LESSONS_PER_DAY_MAX*day_of_week)+lesson_of_day;
    day_of_week++;

    /* Printing the lesson and the teacher */
    print_lesson_teacher(week[lesson_in_individual]);
    print_lesson(week[lesson_in_individual]);

    /* Making new line */
    if (day_of_week == SCHOOL_DAYS_IN_WEEK){
      day_of_week = 0;
      lesson_of_day++;
      /* Making an ekstra new line to act as a break ind school */
      if ((lesson_of_day % 2) == 0){
        printf("\n");
      }
      printf("\n");

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
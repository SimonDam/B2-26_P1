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
#define NUMBER_OF_INDIVIDUALS 10
#define DEFAULT_LENGTH_STRING 200
#define CLASS_NAME_MAX 3

#define FITNESS_LESSONS_IN_ROW 10

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
  char grade[CLASS_NAME_MAX];
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
void calculate_fitness_all(individual individuals[]);
void calculate_fitness_one(individual *indi);
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
  /* init 9 */
  lesson week9[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual9;
  individual individuals9[NUMBER_OF_INDIVIDUALS];
  
  /* init 8 */
  lesson week8[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  individual chosen_individual8;
  individual individuals8[NUMBER_OF_INDIVIDUALS];

  /* init 7 */
  lesson week7[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
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

  printf("  9. klasse: \n");
  create_skema(week9, chosen_individual9, teachers_names);
  print_skema(week9);
  printf("\n\n  8. klasse:\n");
  create_skema(week8, chosen_individual8, teachers_names);
  print_skema(week8);
  printf("\n\n  7. klasse:\n");
  create_skema(week7, chosen_individual7, teachers_names);
  print_skema(week7);
  
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
    if ((temp_name[0] == 'E') && (temp_name[1] == 'N') && (temp_name[2] == 'D')){
      printf("DONE\n");
      done = 1;
    }
    /* Creating the list of names */
    else {
      strcat(teachers_names, temp_name);
      printf("%s  %d -  %s\n", temp_name, res, teachers_names);
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
  for(int j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(int i = 0; i < LESSONS_PER_DAY_MAX; i++){
      result.individual_num[i][j] = rand() % NUMBER_OF_DIFFERENT_LESSONS;
    }
  }
  result.individual_num[0][0] = mat;
  result.individual_num[1][0] = mat;
  result.individual_num[2][0] = mat;
  result.individual_num[3][0] = mat;

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
  for (int j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(int i = 0; i < LESSONS_PER_DAY_MAX; i += 2){
      if (indi->individual_num[i][j] == indi->individual_num[i+1][j]){
        indi->fitness = indi->fitness + FITNESS_LESSONS_IN_ROW;
      }
    }
  }
}

individual choose_individual(individual individuals[]){
  individual chosen;
  chosen.fitness = 0;

  int i = 0;
  int k = 0;
  int j = 0;
  for(k = 0; k < NUMBER_OF_INDIVIDUALS; k++){
    if (individuals[i].fitness > chosen.fitness){
      chosen.fitness = individuals[i].fitness;
      for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
        for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
          chosen.individual_num[i][j] = individuals[k].individual_num[i][j];
        }
      }
    }
  }
  printf("BEST FITNESS: %d\n", chosen.fitness);
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

/* child solution is created by using 2 random days from parrent A, 2 random days from parrent B and then filling in the last day so the required
   ammount of lesson for each class is met */

individual merge_individuals(individual individualA, individual individualB){
  individual new_individual;
  new_individual.grade = individualA.grade;
  int i;

  /* resets all lessons in the new individual */
  for(i = 0; i < LESSONS_PER_WEEK_MAX; i++){
    new_individual.individual_num[i] = -1;
  }

  /* First, two days (1-5) are picked from both parrents, making sure that no parrents deliver the same day */

  int dayA1 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  int dayA2;

  do {
    dayA2 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  } while (dayA2 == dayA1);

  insert_new_days(&new_individual, individualA, dayA1, dayA2);

  int dayB1, dayB2;

  do {
    dayB1 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  } while (dayB1 == dayA1 || dayB1 == dayA2);

  do {
    dayB2 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  } while (dayB2 == dayA1 || dayB2 == dayA2 || dayB2 == dayB1);

  insert_new_days(&new_individual, individualB, dayB1, dayB2);
  complete_missing_day(&new_individual);
  return new_individual;

  /* LESSONS_PER_DAY_MAX */
  /* SCHOOL_DAYS_IN_WEEK */
  /* LESSONS_PER_WEEK_MAX */
}

void insert_new_days(individual * dest_individual, individual deliver_individual, int day1, int day2){
  int count1 = (LESSONS_PER_DAY_MAX * day1) - LESSONS_PER_DAY_MAX;
  int count2 = (LESSONS_PER_DAY_MAX * day2) - LESSONS_PER_DAY_MAX;

  int i;
  for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
    dest_individual->individual_num[count1] = deliver_individual.individual_num[count1];
    dest_individual->individual_num[count2] = deliver_individual.individual_num[count2];

    count1++;
    count2++;
  }
}

void complete_missing_day(individual * incomplete_individual){
  int i;
  int count = -1;

  for(i = 0; i < LESSONS_PER_WEEK_MAX; i++){
    if(incomplete_individual->individual_num[i] == -1){
      count = i;
      break;
    }
  }

  assert(count != -1);

  int danish_req, math_req, english_req, language_req, physics_req, history_req, religion_req, 
      socialstudies_req, geography_req, biology_req, gym_req,
      crafting_req, elective_req;

  switch(incomplete_individual->grade){
    case 7:
    /* Indstil kravet for hver klasse til hver lektion */
      break;
    case 8:
      break;
    case 9:
      break;
    default:
      printf("error \n");
      exit(0);
  }

  for(i = 0; i < LESSONS_PER_WEEK_MAX - LESSONS_PER_DAY_MAX; i++){
    switch(incomplete_individual->individual_num[i]){
      case dan:
        danish_req--;
        break;
      case mat:
        math_req--;
        break;
      case eng:
        english_req--;
        break;
      case tys:
        language_req--;
        break;
      case fys:
        physics_req--;
        break;
      case his:
        history_req--;
        break;
      case sam:
        socialstudies_req--;
        break;
      case valg:
        socialstudies_req--;
        break;
      case geo:
        geography_req--;
        break;
      case bio:
        biology_req--;
        break;
      case gym:
        gym_req--;
        break;
      case fri:
        break;
      case rel:
        religion_req--;
        break;
      case hda:
        crafting_req--;
        break;
    }
  }

  for(i = LESSONS_PER_WEEK_MAX - LESSONS_PER_DAY_MAX; i < LESSONS_PER_WEEK_MAX; i++){
    if(danish_req > 0){
      incomplete_individual->individual_num[i] = dan;
      danish_req--;
    }
    if(english_req > 0){
      incomplete_individual->individual_num[i] = eng;
      english_req--;
    }
    if(language_req > 0){
      incomplete_individual->individual_num[i] = tys;
      language_req--;
    }
    if(history_req > 0){
      incomplete_individual->individual_num[i] = his;
      history_req--;
    }
    if(religion_req > 0){
      incomplete_individual->individual_num[i] = rel;
      religion_req--;
    }
    if(socialstudies_req > 0){
      incomplete_individual->individual_num[i] = sam;
      socialstudies_req--;
    }
    if(math_req > 0){
      incomplete_individual->individual_num[i] = mat;
      math_req--;
    }
    if(geography_req > 0){
      incomplete_individual->individual_num[i] = geo;
      geography_req--;
    }
    if(physics_req > 0){
      incomplete_individual->individual_num[i] = fys;
      physics_req--;
    }
    if(biology_req > 0){
      incomplete_individual->individual_num[i] = bio;
      biology_req--;
    }
    if(crafting_req > 0){
      incomplete_individual->individual_num[i] = hda;
      crafting_req--;
    }
    if(elective_req > 0){
      incomplete_individual->individual_num[i] = valg;
      elective_req--;
    }
    if(gym_req > 0){
      incomplete_individual->individual_num[i] = gym;
      gym_req--;
    }
  }
}
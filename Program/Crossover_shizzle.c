typedef struct requirements reqs;
struct requirements{
  int danish_req;
  int math_req;
  int english_req;
  int language_req;
  int physics_req;
  int history_req; 
  int religion_req;
  int socialstudies_req;
  int geography_req;
  int biology_req;
  int gym_req;
  int crafting_req; 
  int elective_req;
};



/* child solution is created by using 2 random days from parrent A, 2 random days from parrent B and then filling in the last day so the required
   ammount of lesson for each class is met */

individual merge_individuals(individual individualA, individual individualB){

  individual new_individual;

  /* sets childs grade equal to the grade of the first parrents, and makes sure that both parrents have the same grade */

  new_individual.grade = individualA.grade;

  assert(individualA.grade == individualB.grade);

  int i, j;

  /* resets all lessons in the new individual */

  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){

    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){

      new_individual.individual_num[j][i] = fri;
    }
  }

  /* Picks two random days to be used from the first parrent and makes sure, its two different days */

  srand(time(NULL));  

  int dayA1 = rand() % SCHOOL_DAYS_IN_WEEK;
  int dayA2;

  do
  dayA2 = rand() % SCHOOL_DAYS_IN_WEEK;
  while(dayA2 == dayA1);

  /* inserts the chosen days from parrentA into the child on the same days */

  insert_new_days(&new_individual, individualA, dayA1, dayA2);

  /* Picks two other random days from parrentB, to avoid repetition when both parrents are completely or almost identical */

  int dayB1, dayB2;

  do
  dayB1 = rand() % SCHOOL_DAYS_IN_WEEK;
  while(dayB1 == dayA1 || dayB1 == dayA2);

  do
  dayB2 = rand() % SCHOOL_DAYS_IN_WEEK;
  while(dayB2 == dayA1 || dayB2 == dayA2 || dayB2 == dayB1);

  insert_new_days(&new_individual, individualB, dayB1, dayB2);

  /* finds the day that have not yet been filled in the child */

  dayC = 15 - dayA1 - dayA2 - dayB1 - dayB2;

  /* after two days have been inserted from each parrent, the last day is used to fill in the subjects missing according to the official requirements */

  complete_missing_day(&new_individual, dayC);

  /* returns the completed child */

  return new_individual;

}

void insert_new_days(individual * dest_individual, individual deliver_individual, int day1, int day2){


  int i;

  /* inserts day1 and day2 from the deliver individual into day1 and day2 in the dest individual */

  for(i = 0; i < LESSONS_PER_DAY_MAX; i++){

    dest_individual->individual_num[i][day1] = deliver_individual.individual_num[i][day1];
    dest_individual->individual_num[i][day2] = deliver_individual.individual_num[i][day2];

  }
}

void complete_missing_day(individual * incomplete_individual, int emptyday){

  /* local struct that contains the current requirements */

  reqs local_requirements;

  /* finds the requirements for the current grade and saves */

  find_req(incomplete_individual->grade, &local_requirements);

  int i, j;

  /* counts through the entire schedule and counts the required lessons in each subject down one time, everytime it is met, to end up with only the missing lessons
     in the local_requirements */

  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){

    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){

      switch(incomplete_individual->individual_num[i][j]){

      case dan:
        local_requirements->danish_req--;
        break;
      case mat:
        local_requirements->math_req--;
        break;
      case eng:
        local_requirements->english_req--;
        break;
      case tys:
        local_requirements->language_req--;
        break;
      case fys:
        local_requirements->physics_req--;
        break;
      case his:
        local_requirements->history_req--;
        break;
      case sam:
        local_requirements->socialstudies_req--;
        break;
      case valg:
        local_requirements->socialstudies_req--;
        break;
      case geo:
        local_requirements->geography_req--;
        break;
      case bio:
        local_requirements->biology_req--;
        break;
      case gym:
        local_requirements->gym_req--;
        break;
      case fri:
        break;
      case rel:
        local_requirements->religion_req--;
        break;
      case hda:
        local_requirements->crafting_req--;
        break;
      default:
        exit(0);
      }
    }
  }

  /* goes through all lessons in the empty day. If a requirement is not fullfilled, the lesson is added and the remaining requirements is counted one down */  

  for(i = 0; i < LESSONS_PER_DAY_MAX; i++){

    if(local_requirements->danish_req > 0){
      incomplete_individual->individual_num[i][emptyday] = dan;
      local_requirements->danish_req--;
    }
    else if(local_requirements->english_req > 0){
      incomplete_individual->individual_num[i][emptyday] = eng;
      local_requirements->english_req--;
    }
    else if(local_requirements->language_req > 0){
      incomplete_individual->individual_num[i][emptyday] = tys;
      local_requirements->language_req--;
    }
    else if(local_requirements->math_req > 0){
      incomplete_individual->individual_num[i][emptyday] = mat;
      local_requirements->math_req--;
    }
    else if(local_requirements->physics_req > 0){
      incomplete_individual->individual_num[i][emptyday] = fys;
      local_requirements->physics_req--;
    }
    else if(local_requirements->history_req > 0){
      incomplete_individual->individual_num[i][emptyday] = his;
      local_requirements->history_req--;
    }
    else if(local_requirements->religion_req > 0){
      incomplete_individual->individual_num[i][emptyday] = rel;
      local_requirements->religion_req--;
    }
    else if(local_requirements->socialstudies_req > 0){
      incomplete_individual->individual_num[i][emptyday] = sam;
      local_requirements->socialstudies_req--;
    }
    else if(local_requirements->geography_req > 0){
      incomplete_individual->individual_num[i][emptyday] = geo;
      local_requirements->geography_req--;
    }
    else if(local_requirements->biology_req > 0){
      incomplete_individual->individual_num[i][emptyday] = bio;
      local_requirements->biology_req--;
    }
    else if(local_requirements->crafting_req > 0){
      incomplete_individual->individual_num[i][emptyday] = hda;
      local_requirements->crafting_req--;
    }
    else if(local_requirements->elective_req > 0){
      incomplete_individual->individual_num[i][emptyday] = valg;
      local_requirements->elective_req--;
    }
    else if(local_requirements->gym_req > 0){
      incomplete_individual->individual_num[i][emptyday] = gym;
      local_requirements->gym_req--;
    }
  }
  

}

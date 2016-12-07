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


individual merge_individuals(individual individualA, individual individualB){
  individual new_individual;
  new_individual.grade = individualA.grade;
  int i, j;

  /* resets all lessons in the new individual */
  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      new_individual.individual_num[j][i] = fri;
    }
  }

  srand(time(NULL));  /* ############# SKAL VI GØRE DETTE I HVER FUNKTION, DEN SKAL BRUGES?. ELLER KAN DET GØRES GLOBALT? ################# */

  /* First, two days (1-5) are picked from both parrents, making sure that no parrents deliver the same day */
  int dayA1 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  int dayA2;

  do {
    dayA2 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  } while(dayA2 == dayA1);

  insert_new_days(&new_individual, individualA, dayA1, dayA2);
  int dayB1, dayB2;

  do {
    dayB1 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  } while(dayB1 == dayA1 || dayB1 == dayA2);

  do {
    dayB2 = rand() % (SCHOOL_DAYS_IN_WEEK + 1);
  } while(dayB2 == dayA1 || dayB2 == dayA2 || dayB2 == dayB1);

  insert_new_days(&new_individual, individualB, dayB1, dayB2);
  complete_missing_day(&new_individual);
  return new_individual;


// LESSONS_PER_DAY_MAX
// SCHOOL_DAYS_IN_WEEK
// LESSONS_PER_WEEK_MAX
}

void insert_new_days(individual * dest_individual, individual deliver_individual, int day1, int day2){
  int count1 = (LESSONS_PER_DAY_MAX * day1) - LESSONS_PER_DAY_MAX;
  int count2 = (LESSONS_PER_DAY_MAX * day2) - LESSONS_PER_DAY_MAX;
  int i;

  for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
    dest_individual->individual_num[day1][i] = deliver_individual.individual_num[i][day1];
    dest_individual->individual_num[day2][i] = deliver_individual.individual_num[i][day2];
  }
}

void complete_missing_day(individual * incomplete_individual){
  reqs local_requirements;
  find_req(incomplete_individual->grade, local_requirements);
  int i, j;

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

  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){
    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){
      if(local_requirements->danish_req > 0){
        incomplete_individual->individual_num[i][j] = dan;
        local_requirements->danish_req--;
      }
      if(local_requirements->english_req > 0){
        incomplete_individual->individual_num[i][j] = eng;
        local_requirements->english_req--;
      }
      if(local_requirements->language_req > 0){
        incomplete_individual->individual_num[i][j] = tys;
        local_requirements->language_req--;
      }
      if(local_requirements->history_req > 0){
        incomplete_individual->individual_num[i][j] = his;
        local_requirements->history_req--;
      }
      if(local_requirements->religion_req > 0){
        incomplete_individual->individual_num[i][j] = rel;
        local_requirements->religion_req--;
      }
      if(local_requirements->socialstudies_req > 0){
        incomplete_individual->individual_num[i][j] = sam;
        local_requirements->socialstudies_req--;
      }
      if(local_requirements->math_req > 0){
        incomplete_individual->individual_num[i][j] = mat;
        local_requirements->math_req--;
      }
      if(local_requirements->geography_req > 0){
        incomplete_individual->individual_num[i][j] = geo;
        local_requirements->geography_req--;
      }
      if(local_requirements->physics_req > 0){
        incomplete_individual->individual_num[i][j] = fys;
        local_requirements->physics_req--;
      }
      if(local_requirements->biology_req > 0){
        incomplete_individual->individual_num[i][j] = bio;
        local_requirements->biology_req--;
      }
      if(local_requirements->crafting_req > 0){
        incomplete_individual->individual_num[i][j] = hda;
        local_requirements->crafting_req--;
      }
      if(local_requirements->elective_req > 0){
        incomplete_individual->individual_num[i][j] = valg;
        local_requirements->elective_req--;
      }
      if(local_requirements->gym_req > 0){
        incomplete_individual->individual_num[i][j] = gym;
        local_requirements->gym_req--;
      }
    }
  }
}
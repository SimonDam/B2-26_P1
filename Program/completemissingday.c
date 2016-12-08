void complete_missing_day(individual * incomplete_individual, int emptyday, teacher teacher_data[], int number_teachers){

  /* finds the requirements for the current grade and saves */

  reqs local_requirements = find_req(teacher_data, incomplete_individual->grade, number_teachers);

  int i, j;

  /* counts through the entire schedule and counts the required lessons in each subject down one time, everytime it is met, to end up with only the missing lessons
     in the local_requirements */

  for(j = 0; j < SCHOOL_DAYS_IN_WEEK; j++){

    for(i = 0; i < LESSONS_PER_DAY_MAX; i++){

      switch(incomplete_individual->individual_num[i][j]){

      case dan:
        local_requirements.dan_req--;
        break;
      case mat:
        local_requirements.mat_req--;
        break;
      case eng:
        local_requirements.eng_req--;
        break;
      case tys:
        local_requirements.tys_req--;
        break;
      case fys:
        local_requirements.fys_req--;
        break;
      case his:
        local_requirements.his_req--;
        break;
      case sam:
        local_requirements.sam_req--;
        break;
      case valg:
        local_requirements.sam_req--;
        break;
      case geo:
        local_requirements.geo_req--;
        break;
      case bio:
        local_requirements.bio_req--;
        break;
      case gym:
        local_requirements.gym_req--;
        break;
      case fri:
        break;
      case rel:
        local_requirements.rel_req--;
        break;
      case hda:
        local_requirements.pra_req--;
        break;
      default:
        exit(0);
      }
    }
  }

  /* goes through all lessons in the empty day. If a requirement is not fullfilled, the lesson is added and the remaining requirements is counted one down */  

  for(i = 0; i < LESSONS_PER_DAY_MAX; i++){

    if(local_requirements.dan_req > 0){
      incomplete_individual->individual_num[i][emptyday] = dan;
      local_requirements.dan_req--;
    }
    else if(local_requirements.eng_req > 0){
      incomplete_individual->individual_num[i][emptyday] = eng;
      local_requirements.eng_req--;
    }
    else if(local_requirements.tys_req > 0){
      incomplete_individual->individual_num[i][emptyday] = tys;
      local_requirements.tys_req--;
    }
    else if(local_requirements.mat_req > 0){
      incomplete_individual->individual_num[i][emptyday] = mat;
      local_requirements.mat_req--;
    }
    else if(local_requirements.fys_req > 0){
      incomplete_individual->individual_num[i][emptyday] = fys;
      local_requirements.fys_req--;
    }
    else if(local_requirements.his_req > 0){
      incomplete_individual->individual_num[i][emptyday] = his;
      local_requirements.his_req--;
    }
    else if(local_requirements.rel_req > 0){
      incomplete_individual->individual_num[i][emptyday] = rel;
      local_requirements.rel_req--;
    }
    else if(local_requirements.sam_req > 0){
      incomplete_individual->individual_num[i][emptyday] = sam;
      local_requirements.sam_req--;
    }
    else if(local_requirements.geo_req > 0){
      incomplete_individual->individual_num[i][emptyday] = geo;
      local_requirements.geo_req--;
    }
    else if(local_requirements.bio_req > 0){
      incomplete_individual->individual_num[i][emptyday] = bio;
      local_requirements.bio_req--;
    }
    else if(local_requirements.pra_req > 0){
      incomplete_individual->individual_num[i][emptyday] = hda;
      local_requirements.pra_req--;
    }
    else if(local_requirements.val_req > 0){
      incomplete_individual->individual_num[i][emptyday] = valg;
      local_requirements.val_req--;
    }
    else if(local_requirements.gym_req > 0){
      incomplete_individual->individual_num[i][emptyday] = gym;
      local_requirements.gym_req--;
    }
  }
}
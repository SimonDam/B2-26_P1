individual pick_individual(individual individuals[]);

individual pick_individual(individual individuals[]){

  srand(time(NULL));
  
  int i, sum = 0;

  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){

    sum += individuals[i].fitness;
  }

  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){

    if(sum < individuals[i].fitness){

      return individuals[i];
    }
  }
}
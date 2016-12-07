individual pick_individual(individual individuals[]);

individual pick_individual(individual individuals[]){

  srand(time(NULL));
  
  int i, sum = 0;

  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){

    sum += individuals[i].fitness;
  }

  int field = rand() % sum;

  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){

    if(field < individuals[i].fitness){

      return individuals[i];
    }
  }
}
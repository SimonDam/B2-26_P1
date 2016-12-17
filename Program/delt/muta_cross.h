void make_old_population(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS]);
void crossover(individual individuals[][NUMBER_OF_INDIVIDUALS], individual individuals_temp[][NUMBER_OF_INDIVIDUALS], requirements requirements_classes[], int generation);
void crossover_indi(individual individuals[][NUMBER_OF_INDIVIDUALS], individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num, int generation);
void mutation(individual individuals[][NUMBER_OF_INDIVIDUALS]);

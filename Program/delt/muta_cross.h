void crossover(individual **population, individual **old_population, requirements *requirements_classes, int generation);
void crossover_indi(individual **population, individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num, int generation);
void mutation(individual **population);
void make_old_population(individual **population, individual **old_population);


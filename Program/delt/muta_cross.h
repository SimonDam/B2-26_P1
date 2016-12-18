void crossover(individual **population, individual **old_population, requirements *requirements_classes);
void crossover_indi(individual **population, individual parent1, individual parent2, requirements requirements_classes, int class, int indi_num);
void mutation(individual **population);
void make_old_population(individual **population, individual **old_population);
void selektion(individual **population, individual **old_population);
int pick_individual(individual **population, individual **old_population, int class, int indi_num);
void choose_individual(individual **population, individual **chosen_individual, int class, int generation);
void calculate_finess_parallel(individual **population, int sum_parrallel[], int class);
void find_best(individual **chosen_population, individual best_of_best[]);
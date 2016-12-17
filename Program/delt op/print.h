
#include "structs.h"
#include "defines.h"

void find_best(individual chosen_individuals[][NUMBER_OF_GENERATIONS], individual best_of_best[]);
void print_func(individual best_of_best[], requirements requirements_class[], int generation_print, teacher teacher_data[][NUMBER_OF_SUBJECTS]);
void print_teacher_and_lesson(int subject, int class, teacher teacher_data[][NUMBER_OF_SUBJECTS]);
void print_time_func (int i);
void print_class_name(int c);
void print_req(individual chosen_individual, requirements requirements_class);
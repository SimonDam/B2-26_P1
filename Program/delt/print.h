#include "structs.h"
#include "defines.h"

void print_req(individual chosen_individual, requirements requirements_class);
void print_func(individual best_of_best[], requirements requirements_classes[], int generation_print, class_info **class_data);
void print_time_func (int i);
void print_teacher_and_lesson(int subject, int class, class_info **class_data);
void print_class_name(int c);
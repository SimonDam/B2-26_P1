#pragma once

#include "defines.h"

enum lesson_number {dan, mat, eng, tys, fys, his, sam, val, geo, bio, gym, rel, pra, fri};

struct individual{
  int lesson_num[LESSONS_PER_DAY_MAX][SCHOOL_DAYS_IN_WEEK];
  int fitness;
  int perfection;
  int lessons_with_parallel;
  int lessons_with_both;
  int heavy_lesson_after;
  int heavy_lesson_before;
  int teacher_overbooked;
  int best_gena7;
  int best_gena8;
  int best_gena9; 
};

struct class_info{
  int number_of_lessons;
  char padding[2];
  char teacher_name[TEACHER_NAME_MAX];
  char lesson_name[LESSON_NAME_MAX];
  char class_name[TEACHER_NAME_MAX];  
};


struct requirements{
  int Dan_req;
  int Mat_req;
  int Eng_req;
  int Tys_req;
  int Fys_req;
  int His_req;
  int Sam_req;
  int Val_req;
  int Geo_req;
  int Bio_req;
  int Gym_req;
  int Rel_req;
  int Pra_req;
  int Fri_req;
};

typedef struct individual individual;
typedef struct class_info class_info;
typedef struct requirements requirements;
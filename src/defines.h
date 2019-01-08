#ifndef _DEFINES
#define _DEFINES

#define LAYERS  3
#define CONFIG { 5, 4, 3 }

#define HARASS_LVL 3
#define SET_SIZE 1000

#define IMG_SIZE 7
#define INPUT_LENGTH (IMG_SIZE * IMG_SIZE)
#define OUTPUT_LENGTH 3

#define lrn_coef 10.0

extern float shared_d;
extern int shared_a;

#define REFERENCE     0
#define INPUT      1000

#endif
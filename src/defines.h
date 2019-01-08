#ifndef _DEFINES
#define _DEFINES

#define LAYERS  4
#define CONFIG { 20, 15, 10, 3 }

// #define LAYERS  4
// #define CONFIG { 6, 5, 4, 3 }

#define HARASS_LVL 3
#define SET_SIZE 2000

#define IMG_SIZE 7
#define INPUT_LENGTH (IMG_SIZE * IMG_SIZE)
#define OUTPUT_LENGTH 3

#define lrn_coef 20.0

extern float shared_d;
extern int shared_a;

#define REFERENCE     0
#define INPUT      1000
#define BACKPROP   2000

#endif
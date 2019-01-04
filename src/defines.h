#ifndef _DEFINES
#define _DEFINES

#define LAYERS  3
#define CONFIG { 3, 4, 5 }

#define HARASS_LVL 0
#define SET_SIZE 5

#define IMG_SIZE 7
#define INPUT_LENGTH (IMG_SIZE * IMG_SIZE)
#define OUTPUT_LENGTH 3

extern float shared_d;
extern int shared_a;

#define GENERATED 1
#define REFERENCE 2

#endif
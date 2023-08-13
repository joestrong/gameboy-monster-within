#include <stdint.h>

#ifndef VECTOR_H
#define VECTOR_H

struct vector {
    int8_t x;
    int8_t y;
};

typedef struct vector vector;

vector get_normalised_vector(int8_t x, int8_t y);

#endif
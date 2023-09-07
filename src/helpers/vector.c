#pragma bank 255

#include <gb/gb.h>
#include <stdlib.h>
#include "vector.h"

#define ANGLE 10
#define STRAIGHT 3
#define DIAGONAL 2

vector* get_normalised_vector(int8_t x, int8_t y) {
  vector* normalised_vector = malloc(sizeof(vector));

  normalised_vector->x = 0;
  normalised_vector->y = 0;

  if (x > ANGLE) {
    if (y > ANGLE) {
      normalised_vector->x = DIAGONAL;
      normalised_vector->y = DIAGONAL;
    } else if (y < -ANGLE) {
      normalised_vector->x = DIAGONAL;
      normalised_vector->y = -DIAGONAL;
    } else {
      normalised_vector->x = STRAIGHT;
    }
  } else if (x < -ANGLE) {
    if (y > ANGLE) {
      normalised_vector->x = -DIAGONAL;
      normalised_vector->y = DIAGONAL;
    } else if (y < -ANGLE) {
      normalised_vector->x = -DIAGONAL;
      normalised_vector->y = -DIAGONAL;
    } else {
      normalised_vector->x = -STRAIGHT;
    }
  } else {
    if (y > ANGLE) {
      normalised_vector->y = STRAIGHT;
    } else if (y < -ANGLE) {
      normalised_vector->y = -STRAIGHT;
    }
  }

  return normalised_vector;
}
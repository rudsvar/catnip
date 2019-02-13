#include "vertex.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Return 1 if two floats are equal enough
int double_eq(double a, double b) { return fabs(a - b) < 0.0001; }

// Return 1 if the vec3s are equal
int vec3_eq(struct vec3 *a, struct vec3 *b) {
  int xeq = double_eq(a->x, b->x);
  int yeq = double_eq(a->y, b->y);
  int zeq = double_eq(a->z, b->z);
  return xeq && yeq && zeq;
}

int main(int argc, char *argv[]) {
  struct vec3 pos = {1, 0, 0};
  vec3_rotate(&pos, 3.141592 / 2);

  struct vec3 expected = {0, 0, 1};
  if (!vec3_eq(&pos, &expected)) {
    return 1;
  }
}

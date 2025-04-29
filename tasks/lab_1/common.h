#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

void rectangle_step(double **field, uint64_t x_i, uint64_t t_i);
void rectangle_scheme(double **field, uint64_t t_0, uint64_t t_n, uint64_t x_0, uint64_t x_n);

#endif

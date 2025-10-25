#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

void rectangle_step(double **field, int32_t x_i, int32_t t_i);
void rectangle_scheme(double **field, int32_t t_0, int32_t t_n, int32_t x_0, int32_t x_n);

#endif

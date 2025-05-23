#include "common.h"
#include "problem.h"

void rectangle_step(double **field, int32_t x_i, int32_t t_i)
{
    field[t_i][x_i] = 0;
    field[t_i][x_i] += 2 * x_step * t_step * f((x_i - 0.5) * x_step, (t_i - 0.5) * t_step);
    field[t_i][x_i] += x_step * (field[t_i - 1][x_i] - field[t_i][x_i - 1] + field[t_i - 1][x_i - 1]);
    field[t_i][x_i] += t_step * (field[t_i][x_i - 1] - field[t_i - 1][x_i] + field[t_i - 1][x_i - 1]);
    field[t_i][x_i] /= (x_step + t_step);
}

void rectangle_scheme(double **field, int32_t t_0, int32_t t_n, int32_t x_0, int32_t x_n)
{
    for (int32_t t_i = t_0; t_i < t_n + t_0; t_i++)
    {
        for (int32_t x_i = x_0; x_i < x_n + x_0; x_i++)
        {
            rectangle_step(field, x_i, t_i);
        }
    }
}

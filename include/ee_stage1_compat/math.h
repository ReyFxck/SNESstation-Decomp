#ifndef SNESSTATION_EE_STAGE1_MATH_H
#define SNESSTATION_EE_STAGE1_MATH_H

double fabs(double x);
float fabsf(float x);
double sqrt(double x);
float sqrtf(float x);
double sin(double x);
float sinf(float x);
double cos(double x);
float cosf(float x);
double tan(double x);
float tanf(float x);
double atan(double x);
float atanf(float x);
double atan2(double y, double x);
float atan2f(float y, float x);
double floor(double x);
float floorf(float x);
double ceil(double x);
float ceilf(float x);

#define HUGE_VAL  (__builtin_huge_val())
#define HUGE_VALF (__builtin_huge_valf())

#endif

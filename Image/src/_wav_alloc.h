#ifndef _ALLOC_H_
#define _ALLOC_H_


float* allocate_1d_float(int const N);
double* allocate_1d_double(int const N);

int* allocate_1d_int(int const N);
unsigned char* allocate_1d_uchar(int const N);

float** allocate_2d_float(int const N, int const M);
void free_2d_float(float ** const a, int const N);

double** allocate_2d_double(int const N, int const M);
void free_2d_double(double ** const a, int const N);

int** allocate_2d_int(int const N, int const M);
void free_2d_int(int ** const a, int const N);

#endif

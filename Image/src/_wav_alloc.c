#include "_image.h"
#include "_wav_alloc.h"

float** allocate_2d_float(int const N, int const M)
{
	int i;
	float **ppmemory = NULL;
	
	ppmemory = (float **)malloc(N*sizeof(float *));
	if (NULL == ppmemory) return NULL;
	
	for (i = 0; i < N; i++) {
		ppmemory[i] = (float *)calloc(M, sizeof(float));
		if (NULL == ppmemory[i]) goto ERR;
	}
	
	return (float**)ppmemory;
	
ERR:
	free_2d_float(ppmemory, N);
	return NULL;
}

void free_2d_float(float ** const ppmemory, int const N)
{
	int i;
	
	if (ppmemory) {
		for (i = 0; i < N; i++) {
			if (ppmemory[i]) {
				free(ppmemory[i]);
				ppmemory[i] = NULL;
			}
		}
		free(ppmemory);
	}
}

double** allocate_2d_double(int const N, int const M)
{
	int i;
	double **ppmemory = NULL;
	
	ppmemory = (double **)malloc(N*sizeof(double *));
	if (NULL == ppmemory) return NULL;
	
	for (i = 0; i < N; i++) {
		ppmemory[i] = (double *)calloc(M, sizeof(double));
		if (ppmemory[i]) goto ERR;
	}
	
	return (double**)ppmemory;
	
ERR:
	free_2d_double(ppmemory, N);
	return NULL;
}

void free_2d_double(double ** const ppmemory, int const N)
{
	int i;
	
	if (ppmemory) {
		for (i = 0; i < N; i++) {
			if (ppmemory[i]) {
				free(ppmemory[i]);
				ppmemory[i] = NULL;
			}
		}
		free(ppmemory);
	}
}


int** allocate_2d_int(int const N, int const M)
{
	int i;
	int **ppmemory = NULL;
	
	ppmemory = (int **)malloc(N*sizeof(int *));
	if (NULL == ppmemory) return NULL;
	
	for (i = 0; i < N; i++) {
		ppmemory[i] = (int *)calloc(M, sizeof(int));
		if (NULL == ppmemory[i]) goto ERR;
	}
	
	return (int**)ppmemory;

ERR:
	free_2d_int(ppmemory, N);
	return NULL;
}

void free_2d_int(int **const ppmemory, int const N)
{
	int i;
	
	if (ppmemory) {
		for (i = 0; i < N; i++) {
			if (ppmemory[i]) {
				free(ppmemory[i]);
				ppmemory[i] = NULL;
			}
		}
		free(ppmemory);
	}
}

float* allocate_1d_float(int const N)
{
	float *pmemory = NULL;
	
	pmemory = (float *)calloc(N, sizeof(float));
	return (float*)pmemory;
}

double* allocate_1d_double(int const N)
{
	double *pmemory = NULL;
	
	pmemory = (double *)calloc(N, sizeof(double));
	return (double*)pmemory;
}

int* allocate_1d_int(int const N)
{
	int *pmemory = NULL;
	
	pmemory = (int *)calloc(N, sizeof(int));
	return (int*)pmemory;
}

unsigned char* allocate_1d_uchar(int const N)
{
	unsigned char *pmemory = NULL;
	
	pmemory = (unsigned char *)calloc(N, sizeof(unsigned char));
	return (unsigned char*)pmemory;
}

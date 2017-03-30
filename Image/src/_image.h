#ifndef	__IMAGE_H_
#define __IMAGE_H_


#include <math.h>
#include <float.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include "../include/IMAGE.H"


#define PI      3.1415926535897932384626433832795f
#define SQRT2	1.4142135623730950488016887242097f


#pragma warning(disable : 4996)
#pragma warning(disable : 4103)


#ifdef __cplusplus
extern "C" {
#endif


// ���߳�
#ifdef MULTITHREAD
	#define MAX_THREAD_NUM       1024

	typedef struct THREAD {
		pthread_t tid;           // �߳�ID
		sem_t     runsem;        // ��ʼ�źŵ�
		sem_t     donesem;       // ����źŵ�
	} THREAD;

	// �߳���
	extern int threadnum;
#endif


// ���Ǳ�׼Ӱ��ת����IMAGE Library��֧�ֵı�׼Ӱ��
int _converttostdimage(IMAGE * const pimage);

int _makeworkimage(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nsize);

COMPLEX _cadd(COMPLEX const c1, COMPLEX const c2);
COMPLEX _csub(COMPLEX const c1, COMPLEX const c2);
COMPLEX _cmul(COMPLEX const c1, COMPLEX const c2);

int _DCT(float * const f, float * const F, int const power);
int _IDCT(float * const F, float * const f, int const power);

int _FFT(COMPLEX * const TD, COMPLEX * const FD, int const power);
int _IFFT(COMPLEX * const FD, COMPLEX * const TD, int const power);


// ��ѧ����
int _float2int(double a);
int _ceil(double a);
float _fabs(float a);
float _sqrt(float a);
float _rsqrt(float a);


#ifdef __cplusplus
}
#endif

#endif

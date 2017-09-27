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


// 多线程
#ifdef MULTITHREAD
	#define MAX_THREAD_NUM       1024

	typedef struct THREAD {
		pthread_t tid;           // 线程ID
		sem_t     runsem;        // 开始信号灯
		sem_t     donesem;       // 完成信号灯
	} THREAD;

	// 线程数
	extern int threadnum;
#endif


// 将非标准影像转换到IMAGE Library所支持的标准影像
int _converttostdimage(IMAGE * const pimage);

int _makeworkimage(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nsize);

COMPLEX _cadd(COMPLEX const c1, COMPLEX const c2);
COMPLEX _csub(COMPLEX const c1, COMPLEX const c2);
COMPLEX _cmul(COMPLEX const c1, COMPLEX const c2);

int _DCT(float * const f, float * const F, int const power);
int _IDCT(float * const F, float * const f, int const power);

int _FFT(COMPLEX * const TD, COMPLEX * const FD, int const power);
int _IFFT(COMPLEX * const FD, COMPLEX * const TD, int const power);


// 数学函数
int _float2int(double a);
int _ceil(double a);
float _fabs(float a);
float _sqrt(float a);
float _rsqrt(float a);
float _atan2(float y, float x);


#ifdef __cplusplus
}
#endif

#endif

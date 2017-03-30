/////////////////////////////////////////////////////////////////////
// dct.c
//
// 功  能: 离散余弦变换
//
// 修  订: 20:26 2005-1-2
//         12:23 2005-6-19
//===================================================================

#include "_image.h"

int _DCT(float * const f, float * const F, int const power)
{
	int rcode = NO_ERROR;

	int i, count;
	float s;
	COMPLEX *X = NULL;
	
	//
	// 0. 参数安全检查
	//
	assert(f);
	assert(F);
	assert(power >= 0);
	if (NULL == f || NULL == F || power < 0) {
		rcode = ERR_PARAMETER; goto RET;
	}
	
	//
	// 1. 计算离散余弦变换点数
	//
	count = 1 << power;
	
	//
	// 2. 分配运算所需的存储
	//
	X = (COMPLEX *)calloc(count*2, sizeof(COMPLEX));
	if (NULL == X) { rcode = ERR_MEM; goto RET; }
	
	//
	// 3. 初始化
	//
	memset(X, 0, sizeof(COMPLEX)*count*2);
	
	//
	// 4. 将时域点写入存储器
	//
	for (i = 0; i < count; i++) X[i].re = f[i];
	
	//
	// 5. 调用快速傅里叶变换
	//
	rcode = _FFT(X, X, power+1);
	if (rcode != NO_ERROR) goto RET;
	
	//
	// 6. 调整系数
	//
	s    = (float)(1.0/sqrt(1.0*count));
	F[0] = (float)(X[0].re*s);
	s    = (float)(s*SQRT2);
	
	for (i = 1; i < count; i++)
		F[i] = (float)((X[i].re*cos(i*PI/(count*2)) + X[i].im*sin(i*PI/(count*2)))*s);
	
RET:
	if (X) free(X);
	return rcode;	
}

int _IDCT(float * const F, float * const f, int const power)
{
	int rcode = NO_ERROR;

	int i, count;
	float s;
	COMPLEX *X = NULL;
	
	//
	// 0 参数安全检查
	//
	assert(f);
	assert(F);
	assert(power >= 0);
	if (NULL == f || NULL == F || power < 0) {
		rcode = ERR_PARAMETER; goto RET;
	}
	
	//
	// 1 计算离散余弦反变换点数
	//
	count = 1 << power;
	
	//
	// 2 分配运算所需存储
	//
	X = (COMPLEX *)calloc(count*2, sizeof(COMPLEX));
	if (NULL == X) { rcode = ERR_MEM; goto RET; }
	
	//
	// 3 初始化
	//
	memset(X, 0, sizeof(COMPLEX)*count*2);
	
	//
	// 4 调整频域点写入存储
	//
	for (i = 0; i < count; i++) {
		X[i].re = F[i]*cos(i*PI/(count*2));
		X[i].im = F[i]*sin(i*PI/(count*2));
	}
	
	//
	// 5 调用快速傅里叶反变换
	//
	rcode = _IFFT(X, X, power+1);
	if (rcode != NO_ERROR) goto RET;
	
	//
	// 6 调整系数
	//
	s = (float)(1.0/sqrt(1.0*count));
	for (i = 0; i < count; i++)
		f[i] = (float)((1-SQRT2)*s*F[0] + SQRT2*s*X[i].re*count*2);
	
RET:
	if (X) free(X);
	return rcode;
}

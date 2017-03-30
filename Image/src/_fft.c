/////////////////////////////////////////////////////////////////////
// fft.c
//
// ��  ��: ����Ҷ�任
//
// ��  ��: 20:26 2005-1-2
//         12:23 2005-6-19
//===================================================================

#include "_image.h"

int _FFT(COMPLEX * const TD, COMPLEX * const FD, int const power)
{
	int rcode = NO_ERROR;

	int i, j, k, p, bfsize, count;
	float angle;
	COMPLEX *W = NULL, *X1 = NULL, *X2 = NULL, *X = NULL;

	//
	// 0. ������ȫ���
	//
	assert(TD);
	assert(FD);
	assert(power >= 0);
	if (NULL == TD || NULL == FD || power < 0) {
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. ���㸵��Ҷ�任����
	//
	count = 1 << power;

	//
	// 2. ������������洢
	//
	W  = (COMPLEX *)calloc(count/2, sizeof(COMPLEX));
	X1 = (COMPLEX *)calloc(count, sizeof(COMPLEX));
	X2 = (COMPLEX *)calloc(count, sizeof(COMPLEX));
	if (NULL == W || NULL == X1 || NULL == X2) {
		rcode = ERR_MEM; goto RET;
	}

	//
	// 3. �����Ȩϵ��
	//
	for (i = 0; i < count/2; i++) {
		angle = (float)(-i*2*PI/count);
		W[i].re = cos(angle);
		W[i].im = sin(angle);
	}

	//
	// 4. ��ʱ���д��洢��
	//
	memcpy(X1, TD, sizeof(COMPLEX)*count);

	//
	// 5. ��������
	//
	for (k = 0; k < power; k++) {
		for (j = 0; j < (1<<k); j++) {
			bfsize = 1 << (power-k);
			for (i = 0; i < bfsize/2; i++) {
				p = j*bfsize;
				X2[i+p] = _cadd(X1[i+p], X1[i+p+bfsize/2]);
				X2[i+p+bfsize/2] = _cmul(_csub(X1[i+p], X1[i+p+bfsize/2]), W[i*(1<<k)]);
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}

	//
	// 6. ��������
	//
	for (j = 0; j < count; j++) {
		for (p = 0, i = 0; i < power; i++) {
			if (j & (1<<i)) p += 1<<(power-i-1);
		}
		FD[j] = X1[p];
	}

RET:
	if (W)  free(W);
	if (X1) free(X1);
	if (X2) free(X2);
	return rcode;
}

int _IFFT(COMPLEX * const FD, COMPLEX * const TD, int const power)
{
	int rcode = NO_ERROR;

	int i, count;
	COMPLEX *X = NULL;

	//
	// 0. ������ȫ���
	//
	assert(TD);
	assert(FD);
	assert(power >= 0);
	if (NULL == TD || NULL == FD || power < 0) {
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. ���㸵��Ҷ���任����
	//
	count = 1 << power;

	//
	// 2. ��������Ҫ�Ĵ洢
	//
	X = (COMPLEX *)calloc(count, sizeof(COMPLEX));
	if (NULL == X) { rcode = ERR_MEM; goto RET; }

	//
	// 3. ��Ƶ���д��洢��
	//
	memcpy(X, FD, sizeof(COMPLEX)*count);

	//
	// 3. ��Ƶ���Ĺ���
	//
	for (i = 0; i < count; i++)
		X[i].im = -X[i].im;

	//
	// 4. ���ÿ��ٸ���Ҷ�任
	//
	rcode = _FFT(X, TD, power);
	if (rcode != NO_ERROR) goto RET;

	//
	// 5. ��ʱ���Ĺ���
	//
	for (i = 0; i < count; i++) {
		TD[i].re /= count;
		TD[i].im = -TD[i].im/count;
	}

RET:
	if (X) free(X);
	return rcode;
}

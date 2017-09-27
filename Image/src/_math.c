/////////////////////////////////////////////////////////////////////
// _math.c
//
// 功  能: 数学函数
//
// 修  订: 11:52 2004-12-26
//===================================================================

#include "_image.h"

int _float2int(double a)
{
	int i;

	a += 103079215104.0;
	i = ((int*)&a)[0] >> 16;
	if (i < 0) i++;

	return i;
}

int _ceil(double a)
{
	int i;
	double d;

	d = a + 103079215104.0;
	i = ((int*)&d)[0] >> 16;
	if (i < a) i++;

	return i;
}

float _fabs(float a)
{
	int tmp;

	tmp = *(int*)&a;
	tmp &= 0x7FFFFFFF;

	return *(float*)&tmp;
}

float _sqrt(float a)
{
	int *i0, *i1;
	float *f0, *f1;
	float tmp0, tmp1;

	i0 = (int*)&tmp0; f0 = (float*)&tmp0;
	i1 = (int*)&tmp1; f1 = (float*)&tmp1;

	*f0 = a;
	*f1 = a;
	*i0 = 0x1fbcf800 + ((*i0) >> 1);
	*i1 = 0x5f3759df - ((*i1) >> 1);

	return 0.5f * ((*f0) + a * (*f1));
}

float _rsqrt(float a)
{
	int i;
	float b;

	b = a;
	i = *(int*)&b;
	i = 0x5f3759df - (i >> 1);
	b = *(float*)&i;
	b = b * (1.5f - (0.5f * a * b * b));

	return b;
}

float _atan2(float y, float x)
{
	#define _DBL_EPSILON 2.2204460492503131e-16f
	#define atan2_p1     (0.9997878412794807f*57.29577951308232f)
	#define atan2_p3     (-0.3258083974640975f*57.29577951308232f)
	#define atan2_p5     (0.1555786518463281f*57.29577951308232f)
	#define atan2_p7     (-0.04432655554792128f*57.29577951308232f)

	float a, c, c2;
	float ax = (float)fabs(x);
	float ay = (float)fabs(y);

	if (ax >= ay) {
		c = ay / (ax + _DBL_EPSILON);
		c2 = c*c;
		a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
	}
	else {
		c = ax / (ay + _DBL_EPSILON);
		c2 = c*c;
		a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
	}

	if (x < 0) a = 180.f - a;
	if (y < 0) a = 360.f - a;

	return a;
}

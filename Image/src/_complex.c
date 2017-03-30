/////////////////////////////////////////////////////////////////////
// complex.c
//
// 功  能: 复数运算
//
// 修  订: 11:52 2004-12-26
//===================================================================

#include "_image.h"

COMPLEX _cadd(COMPLEX const c1, COMPLEX const c2)
{
	COMPLEX c;

	c.re = c1.re + c2.re;
	c.im = c1.im + c2.im;

	return c;
}

COMPLEX _csub(COMPLEX const c1, COMPLEX const c2)
{
	COMPLEX c;

	c.re = c1.re - c2.re;
	c.im = c1.im - c2.im;

	return c;
}

COMPLEX _cmul(COMPLEX const c1, COMPLEX const c2)
{
	COMPLEX c;

	c.re = c1.re*c2.re - c1.im*c2.im;
	c.im = c1.re*c2.im + c2.re*c1.im;

	return c;
}

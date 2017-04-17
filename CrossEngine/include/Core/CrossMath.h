/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	#define EPSILON_E1  1E-1f
	#define EPSILON_E2  1E-2f
	#define EPSILON_E3  1E-3f
	#define EPSILON_E4  1E-4f
	#define EPSILON_E5  1E-5f
	#define EPSILON_E6  1E-6f
	#define EPSILON_E7  1E-7f
	#define EPSILON_E8  1E-8f
	#define EPSILON_E9  1E-9f

	#define PI          3.14159265359f
	#define INV_PI		0.31830988618f
	#define INV_2PI     0.15915494309f
	#define HALF_PI		1.57079632679f

	#define DEG2RAD(a)  ((a) * PI / 180.0f)
	#define RAD2DEG(a)  ((a) * 180.0f / PI)

	#define M00         0
	#define M01         1
	#define M02         2
	#define M03         3
	#define M10         4
	#define M11         5
	#define M12         6
	#define M13         7
	#define M20         8
	#define M21         9
	#define M22         10
	#define M23         11
	#define M30         12
	#define M31         13
	#define M32         14
	#define M33         15

	typedef struct INT2 {
	public:
		INT2(void)
		{
			m[0] = 0;
			m[1] = 0;
		}

		INT2(const int *vec)
		{
			m[0] = vec[0];
			m[1] = vec[1];
		}

		INT2(const INT2 &vec)
		{
			m[0] = vec.m[0];
			m[1] = vec.m[1];
		}

		INT2(int m0, int m1)
		{
			m[0] = m0;
			m[1] = m1;
		}

		operator int* ()
		{
			return (int *)this;
		}

		operator const int* () const
		{
			return (const int *)this;
		}

		int& operator [] (int index)
		{
			return m[index];
		}

		const int& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				int x, y;
			};

			struct {
				int u, v;
			};

			struct  {
				int s, t;
			};

			int m[2];
		};
	} INT2;

	typedef struct INT3 {
	public:
		INT3(void)
		{
			m[0] = 0;
			m[1] = 0;
			m[2] = 0;
		}

		INT3(const int *vec)
		{
			m[0] = vec[0];
			m[1] = vec[1];
			m[2] = vec[2];
		}

		INT3(const INT3 &vec)
		{
			m[0] = vec.m[0];
			m[1] = vec.m[1];
			m[2] = vec.m[2];
		}

		INT3(int m0, int m1, int m2)
		{
			m[0] = m0;
			m[1] = m1;
			m[2] = m2;
		}

		operator int* ()
		{
			return (int *)this;
		}

		operator const int* () const
		{
			return (const int *)this;
		}

		int& operator [] (int index)
		{
			return m[index];
		}

		const int& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				int x, y, z;
			};

			struct {
				int r, g, b;
			};

			int m[3];
		};
	} INT3;

	typedef struct INT4 {
	public:
		INT4(void)
		{
			m[0] = 0;
			m[1] = 0;
			m[2] = 0;
			m[3] = 1;
		}

		INT4(const int *vec)
		{
			m[0] = vec[0];
			m[1] = vec[1];
			m[2] = vec[2];
			m[3] = vec[3];
		}

		INT4(const INT4 &vec)
		{
			m[0] = vec.m[0];
			m[1] = vec.m[1];
			m[2] = vec.m[2];
			m[3] = vec.m[3];
		}

		INT4(int m0, int m1, int m2, int m3)
		{
			m[0] = m0;
			m[1] = m1;
			m[2] = m2;
			m[3] = m3;
		}

		operator int* ()
		{
			return (int *)this;
		}

		operator const int* () const
		{
			return (const int *)this;
		}

		int& operator [] (int index)
		{
			return m[index];
		}

		const int& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				int x, y, z, w;
			};

			struct {
				int r, g, b, a;
			};

			int m[4];
		};
	} INT4;

	typedef struct FLOAT2 {
	public:
		FLOAT2(void)
		{
			m[0] = 0.0f;
			m[1] = 0.0f;
		}

		FLOAT2(const float *vec)
		{
			m[0] = vec[0];
			m[1] = vec[1];
		}

		FLOAT2(const FLOAT2 &vec)
		{
			m[0] = vec.m[0];
			m[1] = vec.m[1];
		}

		FLOAT2(float m0, float m1)
		{
			m[0] = m0;
			m[1] = m1;
		}

		operator float* ()
		{
			return (float *)this;
		}

		operator const float* () const
		{
			return (const float *)this;
		}

		float& operator [] (int index)
		{
			return m[index];
		}

		const float& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				float x, y;
			};

			struct {
				float u, v;
			};

			struct {
				float s, t;
			};

			float m[2];
		};
	} FLOAT2, VEC2;

	typedef struct FLOAT3 {
	public:
		FLOAT3(void)
		{
			m[0] = 0.0f;
			m[1] = 0.0f;
			m[2] = 0.0f;
		}

		FLOAT3(const float *vec)
		{
			m[0] = vec[0];
			m[1] = vec[1];
			m[2] = vec[2];
		}

		FLOAT3(const FLOAT3 &vec)
		{
			m[0] = vec.m[0];
			m[1] = vec.m[1];
			m[2] = vec.m[2];
		}

		FLOAT3(float m0, float m1, float m2)
		{
			m[0] = m0;
			m[1] = m1;
			m[2] = m2;
		}

		operator float* ()
		{
			return (float *)this;
		}

		operator const float* () const
		{
			return (const float *)this;
		}

		float& operator [] (int index)
		{
			return m[index];
		}

		const float& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				float x, y, z;
			};

			struct {
				float r, g, b;
			};

			float m[3];
		};
	} FLOAT3, VEC3;

	typedef struct FLOAT4 {
	public:
		FLOAT4(void)
		{
			m[0] = 0.0f;
			m[1] = 0.0f;
			m[2] = 0.0f;
			m[3] = 1.0f;
		}

		FLOAT4(const float *vec)
		{
			m[0] = vec[0];
			m[1] = vec[1];
			m[2] = vec[2];
			m[3] = vec[3];
		}

		FLOAT4(const FLOAT4 &vec)
		{
			m[0] = vec.m[0];
			m[1] = vec.m[1];
			m[2] = vec.m[2];
			m[3] = vec.m[3];
		}

		FLOAT4(float m0, float m1, float m2, float m3)
		{
			m[0] = m0;
			m[1] = m1;
			m[2] = m2;
			m[3] = m3;
		}

		operator float* ()
		{
			return (float *)this;
		}

		operator const float* () const
		{
			return (const float *)this;
		}

		float& operator [] (int index)
		{
			return m[index];
		}

		const float& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				float x, y, z, w;
			};

			struct {
				float r, g, b, a;
			};

			float m[4];
		};
	} FLOAT4, VEC4, QUAT;

	typedef struct MATRIX2x2 {
	public:
		MATRIX2x2(void)
		{
			m00 = 1.0f; m01 = 0.0f;
			m10 = 0.0f; m11 = 1.0f;
		}

		MATRIX2x2(const float *mtx)
		{
			m00 = mtx[M00]; m01 = mtx[M01];
			m10 = mtx[M10]; m11 = mtx[M11];
		}

		MATRIX2x2(const MATRIX2x2 &mtx)
		{
			m00 = mtx.m00; m01 = mtx.m01;
			m10 = mtx.m10; m11 = mtx.m11;
		}

		MATRIX2x2(
			float _m00, float _m01,
			float _m10, float _m11)
		{
			m00 = _m00; m01 = _m01;
			m10 = _m10; m11 = _m11;
		}

		operator float* ()
		{
			return (float *)this;
		}

		operator const float* () const
		{
			return (const float *)this;
		}

		float& operator [] (int index)
		{
			return m[index];
		}

		const float& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				float m00, m01;
				float m10, m11;
			};

			float m[4];
		};
	} MATRIX2x2, MATRIX2;

	typedef struct MATRIX3x3 {
	public:
		MATRIX3x3(void)
		{
			m00 = 1.0f; m01 = 0.0f; m02 = 0.0f;
			m10 = 0.0f; m11 = 1.0f; m12 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = 1.0f;
		}

		MATRIX3x3(const float *mtx)
		{
			m00 = mtx[M00]; m01 = mtx[M01]; m02 = mtx[M02];
			m10 = mtx[M10]; m11 = mtx[M11]; m12 = mtx[M12];
			m20 = mtx[M20]; m21 = mtx[M21]; m22 = mtx[M22];
		}

		MATRIX3x3(const MATRIX3x3 &mtx)
		{
			m00 = mtx.m00; m01 = mtx.m01; m02 = mtx.m02;
			m10 = mtx.m10; m11 = mtx.m11; m12 = mtx.m12;
			m20 = mtx.m20; m21 = mtx.m21; m22 = mtx.m22;
		}

		MATRIX3x3(
			float _m00, float _m01, float _m02,
			float _m10, float _m11, float _m12,
			float _m20, float _m21, float _m22)
		{
			m00 = _m00; m01 = _m01; m02 = _m02;
			m10 = _m10; m11 = _m11; m12 = _m12;
			m20 = _m20; m21 = _m21; m22 = _m22;
		}

		operator float* ()
		{
			return (float *)this;
		}

		operator const float* () const
		{
			return (const float *)this;
		}

		float& operator [] (int index)
		{
			return m[index];
		}

		const float& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				float m00, m01, m02;
				float m10, m11, m12;
				float m20, m21, m22;
			};

			float m[9];
		};
	} MATRIX3x3, MATRIX3;

	typedef struct MATRIX4x4 {
	public:
		MATRIX4x4(void)
		{
			m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
			m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
			m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
		}

		MATRIX4x4(const float *mtx)
		{
			m00 = mtx[M00]; m01 = mtx[M01]; m02 = mtx[M02]; m03 = mtx[M03];
			m10 = mtx[M10]; m11 = mtx[M11]; m12 = mtx[M12]; m13 = mtx[M13];
			m20 = mtx[M20]; m21 = mtx[M21]; m22 = mtx[M22]; m23 = mtx[M23];
			m30 = mtx[M30]; m31 = mtx[M31]; m32 = mtx[M32]; m33 = mtx[M33];
		}

		MATRIX4x4(const MATRIX4x4 &mtx)
		{
			m00 = mtx.m00; m01 = mtx.m01; m02 = mtx.m02; m03 = mtx.m03;
			m10 = mtx.m10; m11 = mtx.m11; m12 = mtx.m12; m13 = mtx.m13;
			m20 = mtx.m20; m21 = mtx.m21; m22 = mtx.m22; m23 = mtx.m23;
			m30 = mtx.m30; m31 = mtx.m31; m32 = mtx.m32; m33 = mtx.m33;
		}

		MATRIX4x4(
			float _m00, float _m01, float _m02, float _m03,
			float _m10, float _m11, float _m12, float _m13,
			float _m20, float _m21, float _m22, float _m23,
			float _m30, float _m31, float _m32, float _m33)
		{
			m00 = _m00; m01 = _m01; m02 = _m02; m03 = _m03;
			m10 = _m10; m11 = _m11; m12 = _m12; m13 = _m13;
			m20 = _m20; m21 = _m21; m22 = _m22; m23 = _m23;
			m30 = _m30; m31 = _m31; m32 = _m32; m33 = _m33;
		}

		operator float* ()
		{
			return (float *)this;
		}

		operator const float* () const
		{
			return (const float *)this;
		}

		float& operator [] (int index)
		{
			return m[index];
		}

		const float& operator [] (int index) const
		{
			return m[index];
		}

	public:
		union {
			struct {
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};

			float m[16];
		};
	} MATRIX4x4, MATRIX4;

	typedef enum {
		PLANE_SIDE_CROSS = 0,
		PLANE_SIDE_IN,
		PLANE_SIDE_BACK,
		PLANE_SIDE_FRONT
	} PLANE_SIDE;

	typedef struct PLANE {
	public:
		PLANE(void)
		{
			vertex[0] = 0.0f;
			vertex[1] = 0.0f;
			vertex[2] = 0.0f;

			normal[0] = 0.0f;
			normal[1] = 1.0f;
			normal[2] = 0.0f;
		}

		PLANE(const PLANE &plane)
		{
			vertex[0] = plane.vertex[0];
			vertex[1] = plane.vertex[1];
			vertex[2] = plane.vertex[2];

			normal[0] = plane.normal[0];
			normal[1] = plane.normal[1];
			normal[2] = plane.normal[2];
		}

		PLANE(const VEC3 &v, const VEC3 &n)
		{
			vertex[0] = v[0];
			vertex[1] = v[1];
			vertex[2] = v[2];

			normal[0] = n[0];
			normal[1] = n[1];
			normal[2] = n[2];
		}

	public:
		VEC3 vertex;
		VEC3 normal;
	} PLANE;

	typedef struct RAY {
	public:
		RAY(void)
		{
			origin[0] = 0.0f;
			origin[1] = 0.0f;
			origin[2] = 0.0f;

			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 1.0f;
		}

		RAY(const RAY &ray)
		{
			origin[0] = ray.origin[0];
			origin[1] = ray.origin[1];
			origin[2] = ray.origin[2];

			direction[0] = ray.direction[0];
			direction[1] = ray.direction[1];
			direction[2] = ray.direction[2];
		}

		RAY(const VEC3 &o, const VEC3 &dir)
		{
			origin[0] = o[0];
			origin[1] = o[1];
			origin[2] = o[2];

			direction[0] = dir[0];
			direction[1] = dir[1];
			direction[2] = dir[2];
		}

	public:
		VEC3 origin;
		VEC3 direction;
	} RAY;

	typedef struct SPHERE {
	public:
		SPHERE(void)
		{
			center[0] = 0.0f;
			center[1] = 0.0f;
			center[2] = 0.0f;

			radius = 1.0f;
			radius2 = 1.0f;
		}

		SPHERE(const SPHERE &sphere)
		{
			center[0] = sphere.center[0];
			center[1] = sphere.center[1];
			center[2] = sphere.center[2];

			radius = sphere.radius;
			radius2 = sphere.radius2;
		}

		SPHERE(const VEC3 &c, float r, float r2)
		{
			center[0] = c[0];
			center[1] = c[1];
			center[2] = c[2];

			radius = r;
			radius2 = r2;
		}

	public:
		VEC3 center;
		float radius;
		float radius2;
	} SPHERE;

	typedef struct AABB {
	public:
		AABB(void)
		{
			center[0] = 0.0f;
			center[1] = 0.0f;
			center[2] = 0.0f;

			minVertex[0] = 0.0f;
			minVertex[1] = 0.0f;
			minVertex[2] = 0.0f;

			maxVertex[0] = 0.0f;
			maxVertex[1] = 0.0f;
			maxVertex[2] = 0.0f;
		}

		AABB(const AABB &aabb)
		{
			center[0] = aabb.center[0];
			center[1] = aabb.center[1];
			center[2] = aabb.center[2];

			minVertex[0] = aabb.minVertex[0];
			minVertex[1] = aabb.minVertex[1];
			minVertex[2] = aabb.minVertex[2];

			maxVertex[0] = aabb.maxVertex[0];
			maxVertex[1] = aabb.maxVertex[1];
			maxVertex[2] = aabb.maxVertex[2];
		}

		AABB(const VEC3 &c, const VEC3 &min, const VEC3 &max)
		{
			center[0] = c[0];
			center[1] = c[1];
			center[2] = c[2];

			minVertex[0] = min[0];
			minVertex[1] = min[1];
			minVertex[2] = min[2];

			maxVertex[0] = max[0];
			maxVertex[1] = max[1];
			maxVertex[2] = max[2];
		}

	public:
		VEC3 center;
		VEC3 minVertex, maxVertex;
	} AABB;


	extern const VEC3 axisx;
	extern const VEC3 axisy;
	extern const VEC3 axisz;
	extern const QUAT quatIdentity;
	extern const MATRIX4 mtxIdentity;


	///////////////////////////////////////////////////////////////////////////////
	// 数值计算
	//=============================================================================
	inline void RandomSeed(DWORD dwSeek)
	{
		PseudoRandomSeed(dwSeek);
	}

	inline int RandomValue(void)
	{
		return PseudoRandomValue();
	}

	inline float FRandomValue(void)
	{
		return (PseudoRandomValue() % 10000) / 10000.0f;
	}

	inline int FastAbs(int a)
	{
		return abs(a);
	}

	inline float FastFAbs(float a)
	{
		int b;

		b = *(int *)&a;
		b &= 0x7FFFFFFF;

		return *(float *)&b;
	}

	inline int FastCeil(double a)
	{
		int b;
		double c;

		c = a + 103079215104.0;
		b = ((int *)&c)[0] >> 16;
		if (b < a) b++;

		return b;
	}

	inline int FastFloor(double a)
	{
		int b;
		double c;

		c = a + 103079215104.0;
		b = ((int *)&c)[0] >> 16;

		return b;
	}

	inline float FastSqrt(float a)
	{
		/*
		int *i0 = NULL;
		int *i1 = NULL;
		float *f0 = NULL;
		float *f1 = NULL;
		float tmp0, tmp1;

		i0 = (int *)&tmp0; f0 = (float *)&tmp0;
		i1 = (int *)&tmp1; f1 = (float *)&tmp1;

		*f0 = a;
		*f1 = a;
		*i0 = 0x1fbcf800 + ((*i0) >> 1);
		*i1 = 0x5f3759df - ((*i1) >> 1);

		return 0.5f * ((*f0) + a * (*f1));
		*/
		return sqrtf(a);
	}

	inline float FastRSqrt(float a)
	{
		/*
		int b;
		float c;

		c = a;
		b = *(int *)&c;
		b = 0x5f3759df - (b >> 1);
		c = *(float *)&b;
		c = c * (1.5f - (0.5f * a * c * c));

		return c;
		*/
		return 1.0f / sqrtf(a);
	}

	inline float FastPow(float a, float b)
	{
		return powf(a, b);
	}

	inline float FastLog(float a)
	{
		return logf(a);
	}

	inline float FastExp(float a)
	{
		return expf(a);
	}

	inline float FastSin(float a)
	{
		return sinf(a);
	}

	inline float FastCos(float a)
	{
		return cosf(a);
	}

	inline float FastTan(float a)
	{
		return tanf(a);
	}

	inline float FastASin(float a)
	{
		if (a < -1.0f) a = -1.0f;
		if (a >  1.0f) a =  1.0f;
		return asinf(a);
	}

	inline float FastACos(float a)
	{
		if (a < -1.0f) a = -1.0f;
		if (a >  1.0f) a =  1.0f;
		return acosf(a);
	}

	inline float FastATan(float a)
	{
		return atanf(a);
	}

	inline float FastATan2(float x, float y)
	{
		return atan2f(x, y);
	}

	inline float FastClamp(float a, float min = 0.0f, float max = 1.0f)
	{
		if (a < min) a = min;
		if (a > max) a = max;
		return a;
	}

	inline float LerpLinear(float in0, float in1, float t)
	{
		return in0 + (in1 - in0)*t;
	}

	inline float LerpAngle(float in0, float in1, float t)
	{
		if (FastFAbs(in0 - in1) > 180.0f) {
			if (in0 > 180.0f) in0 -= 360.0f;
			if (in1 > 180.0f) in1 -= 360.0f;
		}

		return in0 + (in1 - in0)*t;
	}

	inline float LerpRadian(float in0, float in1, float t)
	{
		if (FastFAbs(in0 - in1) > PI) {
			if (in0 > PI) in0 -= 2.0f*PI;
			if (in1 > PI) in1 -= 2.0f*PI;
		}

		return in0 + (in1 - in0)*t;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 向量运算
	//=============================================================================
	inline void Vec2Zero(VEC2 *vec)
	{
		(*vec)[0] = 0.0f;
		(*vec)[1] = 0.0f;
	}

	inline void Vec3Zero(VEC3 *vec)
	{
		(*vec)[0] = 0.0f;
		(*vec)[1] = 0.0f;
		(*vec)[2] = 0.0f;
	}

	inline void Vec4Zero(VEC4 *vec)
	{
		(*vec)[0] = 0.0f;
		(*vec)[1] = 0.0f;
		(*vec)[2] = 0.0f;
		(*vec)[3] = 0.0f;
	}

	inline VEC2* Vec2Set(VEC2 *vec, float m0, float m1)
	{
		(*vec)[0] = m0;
		(*vec)[1] = m1;

		return vec;
	}

	inline VEC3* Vec3Set(VEC3 *vec, float m0, float m1, float m2)
	{
		(*vec)[0] = m0;
		(*vec)[1] = m1;
		(*vec)[2] = m2;

		return vec;
	}

	inline VEC4* Vec4Set(VEC4 *vec, float m0, float m1, float m2, float m3)
	{
		(*vec)[0] = m0;
		(*vec)[1] = m1;
		(*vec)[2] = m2;
		(*vec)[3] = m3;

		return vec;
	}

	inline VEC2* Vec2Copy(VEC2 *vecOut, const VEC2 *vecIn)
	{
		(*vecOut)[0] = (*vecIn)[0];
		(*vecOut)[1] = (*vecIn)[1];

		return vecOut;
	}

	inline VEC3* Vec3Copy(VEC3 *vecOut, const VEC3 *vecIn)
	{
		(*vecOut)[0] = (*vecIn)[0];
		(*vecOut)[1] = (*vecIn)[1];
		(*vecOut)[2] = (*vecIn)[2];

		return vecOut;
	}

	inline VEC4* Vec4Copy(VEC4 *vecOut, const VEC4 *vecIn)
	{
		(*vecOut)[0] = (*vecIn)[0];
		(*vecOut)[1] = (*vecIn)[1];
		(*vecOut)[2] = (*vecIn)[2];
		(*vecOut)[3] = (*vecIn)[3];

		return vecOut;
	}

	inline void Vec2Clamp(VEC2 *vec, float min = 0.0f, float max = 1.0f)
	{
		(*vec)[0] = FastClamp((*vec)[0], min, max);
		(*vec)[1] = FastClamp((*vec)[1], min, max);
	}

	inline void Vec3Clamp(VEC3 *vec, float min = 0.0f, float max = 1.0f)
	{
		(*vec)[0] = FastClamp((*vec)[0], min, max);
		(*vec)[1] = FastClamp((*vec)[1], min, max);
		(*vec)[2] = FastClamp((*vec)[2], min, max);
	}

	inline void Vec4Clamp(VEC4 *vec, float min = 0.0f, float max = 1.0f)
	{
		(*vec)[0] = FastClamp((*vec)[0], min, max);
		(*vec)[1] = FastClamp((*vec)[1], min, max);
		(*vec)[2] = FastClamp((*vec)[2], min, max);
		(*vec)[3] = FastClamp((*vec)[3], min, max);
	}

	inline float Vec3Length(const VEC3 *vec)
	{
		return FastSqrt((*vec)[0] * (*vec)[0] + (*vec)[1] * (*vec)[1] + (*vec)[2] * (*vec)[2]);
	}

	inline float Vec3Length(const VEC3 *start, const VEC3 *end)
	{
		float x = (*start)[0] - (*end)[0];
		float y = (*start)[1] - (*end)[1];
		float z = (*start)[2] - (*end)[2];

		return FastSqrt(x * x + y * y + z * z);
	}

	inline float Vec3Length2(const VEC3 *vec)
	{
		return (*vec)[0] * (*vec)[0] + (*vec)[1] * (*vec)[1] + (*vec)[2] * (*vec)[2];
	}

	inline float Vec3Length2(const VEC3 *start, const VEC3 *end)
	{
		float x = (*start)[0] - (*end)[0];
		float y = (*start)[1] - (*end)[1];
		float z = (*start)[2] - (*end)[2];

		return x * x + y * y + z * z;
	}

	inline float Vec3Normalize(VEC3 *vec)
	{
		float len;
		float invLen;

		len = Vec3Length(vec);

		if (len > EPSILON_E3) {
			invLen = 1.0f / len;

			(*vec)[0] *= invLen;
			(*vec)[1] *= invLen;
			(*vec)[2] *= invLen;
		}
		else {
			(*vec)[0] = 0.0f;
			(*vec)[1] = 0.0f;
			(*vec)[2] = 0.0f;
		}

		return len;
	}

	inline float Vec3Normalize(VEC3 *vecOut, const VEC3 *vecIn)
	{
		Vec3Copy(vecOut, vecIn);
		return Vec3Normalize(vecOut);
	}

	inline VEC3* Vec3Add(VEC3 *vecOut, const VEC3 *vecIn1, const VEC3 *vecIn2)
	{
		(*vecOut)[0] = (*vecIn1)[0] + (*vecIn2)[0];
		(*vecOut)[1] = (*vecIn1)[1] + (*vecIn2)[1];
		(*vecOut)[2] = (*vecIn1)[2] + (*vecIn2)[2];

		return vecOut;
	}

	inline VEC3* Vec3Sub(VEC3 *vecOut21, const VEC3 *vecIn1, const VEC3 *vecIn2)
	{
		(*vecOut21)[0] = (*vecIn1)[0] - (*vecIn2)[0];
		(*vecOut21)[1] = (*vecIn1)[1] - (*vecIn2)[1];
		(*vecOut21)[2] = (*vecIn1)[2] - (*vecIn2)[2];

		return vecOut21;
	}

	inline VEC3* Vec3Scale(VEC3 *vecOut, const VEC3 *vecIn, float scale)
	{
		(*vecOut)[0] = (*vecIn)[0] * scale;
		(*vecOut)[1] = (*vecIn)[1] * scale;
		(*vecOut)[2] = (*vecIn)[2] * scale;

		return vecOut;
	}

	inline VEC3* Vec3Mul(VEC3 *vecOut, const VEC3 *vecIn1, const VEC3 *vecIn2)
	{
		(*vecOut)[0] = (*vecIn1)[0] * (*vecIn2)[0];
		(*vecOut)[1] = (*vecIn1)[1] * (*vecIn2)[1];
		(*vecOut)[2] = (*vecIn1)[2] * (*vecIn2)[2];

		return vecOut;
	}

	inline VEC3* Vec3Div(VEC3 *vecOut, const VEC3 *vecIn1, const VEC3 *vecIn2)
	{
		(*vecOut)[0] = (*vecIn1)[0] / (*vecIn2)[0];
		(*vecOut)[1] = (*vecIn1)[1] / (*vecIn2)[1];
		(*vecOut)[2] = (*vecIn1)[2] / (*vecIn2)[2];

		return vecOut;
	}

	inline VEC3* Vec3Cross(VEC3 *vecOut, const VEC3 *vecIn1, const VEC3 *vecIn2)
	{
#if defined _ARM_NEON_

	#if defined __LP64__

		__asm__ volatile(
			"ld1 {v0.2s}, [%2]            \n\t"
			"ld1 {v0.s}[2], [%1]          \n\t"
			"mov v0.s[3], v0.s[0]         \n\t" // q0 = (v1y, v1z, v1x, v1x)

			"ld1 {v1.4s}, [%3]            \n\t"
			"mov v1.s[3], v1.s[0]         \n\t" // q1 = (v2x, v2y, v2z, v2x)

			"fmul v2.4s, v0.4s, v1.4s     \n\t" // x = v1y * v2z, y = v1z * v2x

			"mov v0.s[0], v0.s[1]         \n\t"
			"mov v0.s[1], v0.s[2]         \n\t"
			"mov v0.s[2], v0.s[3]         \n\t"

			"mov v1.s[3], v1.s[2]         \n\t"

			"fmul v0.4s, v0.4s, v1.4s     \n\t"

			"mov v0.s[3], v0.s[1]         \n\t"
			"mov v0.s[1], v0.s[2]         \n\t"
			"mov v0.s[2], v0.s[0]         \n\t"

			"fsub v2.4s, v0.4s, v2.4s     \n\t"

			"mov v2.s[0], v2.s[1]         \n\t"
			"mov v2.s[1], v2.s[2]         \n\t"
			"mov v2.s[2], v2.s[3]         \n\t"

			"st1 {v2.2s}, [%0], 8         \n\t" // V[x, y]
			"st1 {v2.s}[2], [%0]          \n\t" // V[z]

			:
			: "r"(vecOut), "r"(vecIn1), "r"((vecIn1 + 1)), "r"(vecIn2), "r"((vecIn2 + 1))
			: "v0", "v1", "v2", "memory"
		);

	#else

		__asm__ volatile(
			"vld1.32 {d1[1]}, [%1]        \n\t"
			"vld1.32 {d0}, [%2]           \n\t"
			"vmov.f32 s2, s1              \n\t" // q0 = (v1y, v1z, v1z, v1x)

			"vld1.32 {d2[1]}, [%3]        \n\t"
			"vld1.32 {d3}, [%4]           \n\t"
			"vmov.f32 s4, s7              \n\t" // q1 = (v2z, v2x, v2y, v2z)

			"vmul.f32 d4, d0, d2          \n\t" // x = v1y * v2z, y = v1z * v2x
			"vmls.f32 d4, d1, d3          \n\t" // x -= v1z * v2y, y-= v1x - v2z

			"vmul.f32 d5, d3, d1[1]       \n\t" // z = v1x * v2y
			"vmls.f32 d5, d0, d2[1]       \n\t" // z-= v1y * vx

			"vst1.32 {d4}, [%0]!          \n\t" // V[x, y]
			"vst1.32 {d5[0]}, [%0]        \n\t" // V[z]

			:
			: "r"(vecOut), "r"(vecIn1), "r"((vecIn1 + 1)), "r"(vecIn2), "r"((vecIn2 + 1))
			: "q0", "q1", "q2", "memory"
		);

	#endif

#else

		(*vecOut)[0] = (*vecIn1)[1] * (*vecIn2)[2] - (*vecIn1)[2] * (*vecIn2)[1];
		(*vecOut)[1] = (*vecIn1)[2] * (*vecIn2)[0] - (*vecIn1)[0] * (*vecIn2)[2];
		(*vecOut)[2] = (*vecIn1)[0] * (*vecIn2)[1] - (*vecIn1)[1] * (*vecIn2)[0];

#endif

		return vecOut;
	}

	inline float Vec3Dot(const VEC3 *vecIn1, const VEC3 *vecIn2)
	{
		return (*vecIn1)[0] * (*vecIn2)[0] + (*vecIn1)[1] * (*vecIn2)[1] + (*vecIn1)[2] * (*vecIn2)[2];
	}

	inline VEC3* Vec3Ma(VEC3 *vecOutEnd, const VEC3 *vecInStart, const VEC3 *vecInDir, float s)
	{
		//
		// |   __ vecEnd = vecStart + s*vecDir
		// |  |\
		// |    \
		// |     \__ vecDir
		// |     |\
		// |       \  vecStart
		// |________________
		//

		(*vecOutEnd)[0] = (*vecInStart)[0] + (*vecInDir)[0] * s;
		(*vecOutEnd)[1] = (*vecInStart)[1] + (*vecInDir)[1] * s;
		(*vecOutEnd)[2] = (*vecInStart)[2] + (*vecInDir)[2] * s;

		return vecOutEnd;
	}

	inline VEC3* Vec3Prep(VEC3 *vecOutPrep, const VEC3 *vecIn, const VEC3 *vecInNormal)
	{
		//
		//   v__
		//     /|\
		//    / | vecPrep
		//   /  |
		//  /   |
		// /    |
		// ----->----> normal
		//  dot*normal
		//

		float dot;

		dot = Vec3Dot(vecIn, vecInNormal);

		(*vecOutPrep)[0] = (*vecIn)[0] - (*vecInNormal)[0] * dot;
		(*vecOutPrep)[1] = (*vecIn)[1] - (*vecInNormal)[1] * dot;
		(*vecOutPrep)[2] = (*vecIn)[2] - (*vecInNormal)[2] * dot;

		return vecOutPrep;
	}

	inline VEC3* Vec3Project(VEC3 *vecOutProject, const VEC3 *vecIn, const VEC3 *vecInNormal)
	{
		//
		//   v__
		//     /|
		//    / |
		//   /  |
		//  /   |
		// /    |
		// ----->----> normal
		//  project = dot*normal
		//

		float dot;

		dot = Vec3Dot(vecIn, vecInNormal);

		(*vecOutProject)[0] = (*vecInNormal)[0] * dot;
		(*vecOutProject)[1] = (*vecInNormal)[1] * dot;
		(*vecOutProject)[2] = (*vecInNormal)[2] * dot;

		return vecOutProject;
	}

	inline VEC3* Vec3Reflect(VEC3 *vecOutReflect, const VEC3 *vecIn, const VEC3 *vecInNormal)
	{
		//               __
		//                /|\
		// vecOutReflect / |
		//              /  |
		//             /   | 2*(vecIn*normal)
		//            /_   |   __
		//           |\   /|\   /|
		//             \   |   /
		//              \  |  /
		//         vecIn \ | / vecOutReflect
		//                \|/
		//

		float dot2;

		dot2 = 2.0f * Vec3Dot(vecIn, vecInNormal);

		(*vecOutReflect)[0] = (*vecInNormal)[0] * dot2 - (*vecIn)[0];
		(*vecOutReflect)[1] = (*vecInNormal)[1] * dot2 - (*vecIn)[1];
		(*vecOutReflect)[2] = (*vecInNormal)[2] * dot2 - (*vecIn)[2];

		return vecOutReflect;
	}

	inline VEC2* Vec2Lerp(VEC2 *vecOut, const VEC2 *vecIn0, const VEC2 *vecIn1, float t)
	{
		(*vecOut)[0] = (*vecIn0)[0] + ((*vecIn1)[0] - (*vecIn0)[0])*t;
		(*vecOut)[1] = (*vecIn0)[1] + ((*vecIn1)[1] - (*vecIn0)[1])*t;

		return vecOut;
	}

	inline VEC3* Vec3Lerp(VEC3 *vecOut, const VEC3 *vecIn0, const VEC3 *vecIn1, float t)
	{
		(*vecOut)[0] = (*vecIn0)[0] + ((*vecIn1)[0] - (*vecIn0)[0])*t;
		(*vecOut)[1] = (*vecIn0)[1] + ((*vecIn1)[1] - (*vecIn0)[1])*t;
		(*vecOut)[2] = (*vecIn0)[2] + ((*vecIn1)[2] - (*vecIn0)[2])*t;

		return vecOut;
	}
	/*
	inline PxVec3 PxVec3Lerp(const PxVec3 &vecIn0, const PxVec3 &vecIn1, float t)
	{
		PxVec3 out;

		out.x = vecIn0.x + (vecIn1.x - vecIn0.x)*t;
		out.y = vecIn0.y + (vecIn1.y - vecIn0.y)*t;
		out.z = vecIn0.z + (vecIn1.z - vecIn0.z)*t;

		return out;
	}
	*/
	inline VEC4* Vec4Lerp(VEC4 *vecOut, const VEC4 *vecIn0, const VEC4 *vecIn1, float t)
	{
		(*vecOut)[0] = (*vecIn0)[0] + ((*vecIn1)[0] - (*vecIn0)[0])*t;
		(*vecOut)[1] = (*vecIn0)[1] + ((*vecIn1)[1] - (*vecIn0)[1])*t;
		(*vecOut)[2] = (*vecIn0)[2] + ((*vecIn1)[2] - (*vecIn0)[2])*t;
		(*vecOut)[3] = (*vecIn0)[3] + ((*vecIn1)[3] - (*vecIn0)[3])*t;

		return vecOut;
	}

	inline void Vec3ToAngleXYZ(float *radXOut, float *radYOut, float *radZOut, const VEC3 *vecIn)
	{
		float len;

		len = FastSqrt((*vecIn)[0] * (*vecIn)[0] + (*vecIn)[2] * (*vecIn)[2]);

		if (len > EPSILON_E3) {
			if (radXOut) *radXOut = FastATan2(-(*vecIn)[1], len);
			if (radYOut) *radYOut = FastATan2((*vecIn)[0], (*vecIn)[2]);
			if (radZOut) *radZOut = 0.0f;
		}
		else {
			if ((*vecIn)[1] > EPSILON_E3) {
				if (radXOut) *radXOut = -HALF_PI;
			}
			else if ((*vecIn)[1] < -EPSILON_E3) {
				if (radXOut) *radXOut = HALF_PI;
			}
			else {
				if (radXOut) *radXOut = 0.0f;
			}

			if (radYOut) *radYOut = 0.0f;
			if (radZOut) *radZOut = 0.0f;
		}
	}

	inline QUAT* Vec3ToQuat(QUAT *quatOut, const VEC3 *vecIn)
	{
		float radX, radY, radZ;
		float cx, cy, cz, sx, sy, sz, cycz, sysz, sycz, cysz;

		Vec3ToAngleXYZ(&radX, &radY, &radZ, vecIn);

		radX *= 0.5f;
		radY *= 0.5f;
		radZ *= 0.5f;

		sx = FastSin(radX);
		sy = FastSin(radY);
		sz = FastSin(radZ);

		cx = FastCos(radX);
		cy = FastCos(radY);
		cz = FastCos(radZ);

		sysz = sy*sz;
		cycz = cy*cz;
		sycz = sy*cz;
		cysz = cy*sz;

		(*quatOut)[0] = sx*cycz - cx*sysz;
		(*quatOut)[1] = cx*sycz + sx*cysz;
		(*quatOut)[2] = cx*cysz - sx*sycz;
		(*quatOut)[3] = cx*cycz + sx*sysz;

		return quatOut;
	}

	inline void Vec3ColorToBytes(const VEC3 *color, BYTE *bytes)
	{
		bytes[0] = (BYTE)((*color)[0] * 255);
		bytes[1] = (BYTE)((*color)[1] * 255);
		bytes[2] = (BYTE)((*color)[2] * 255);
	}

	inline void Vec3BytesToColor(VEC3 *color, BYTE *bytes)
	{
		(*color)[0] = bytes[0] / 255.0f;
		(*color)[1] = bytes[1] / 255.0f;
		(*color)[2] = bytes[2] / 255.0f;
	}

	inline void Vec3DirectionToLatLong(const VEC3 *direction, BYTE *bytes)
	{
		if ((*direction)[0] == 0.0f && (*direction)[1] == 0.0f) {
			if ((*direction)[2] > 0.0f) {
				bytes[0] = 0;
				bytes[1] = 0;
			}
			else {
				bytes[0] = 0;
				bytes[1] = 128;
			}
		}
		else {
			bytes[0] = (int)(RAD2DEG(FastATan2((*direction)[1], (*direction)[0])) * (255.0f / 360.0f)) & 0xff;
			bytes[1] = (int)(RAD2DEG(FastACos((*direction)[2])) * (255.0f / 360.0f)) & 0xff;
		}
	}

	inline void Vec3LatLongToDirection(VEC3 *direction, BYTE *bytes)
	{
		float lat = DEG2RAD((bytes[0] * 360.0f / 255.0f));
		float lng = DEG2RAD((bytes[1] * 360.0f / 255.0f));
		float latSin = (float)FastSin(lat);
		float latCos = (float)FastCos(lat);
		float lngSin = (float)FastSin(lng);
		float lngCos = (float)FastCos(lng);

		(*direction)[0] = latCos * lngSin;
		(*direction)[1] = latSin * lngSin;
		(*direction)[2] = lngCos;
	}


	///////////////////////////////////////////////////////////////////////////////
	// 矩阵运算
	//        - _              _     -
	//       | | m00, m01, m02, | m03 |
	//       | | m10, m11, m12, | m13 |
	//       | |_m20, m21, m22,_| m23 |
	//       |_  m30, m31, m32,   m33_|
	//
	// 其中 tx = m30, ty = m31, tz = m32
	//=============================================================================
	inline void MtxIdentity(MATRIX4 *mtx)
	{
		(*mtx)[M00] = 1.0f; (*mtx)[M01] = 0.0f; (*mtx)[M02] = 0.0f; (*mtx)[M03] = 0.0f;
		(*mtx)[M10] = 0.0f; (*mtx)[M11] = 1.0f; (*mtx)[M12] = 0.0f; (*mtx)[M13] = 0.0f;
		(*mtx)[M20] = 0.0f; (*mtx)[M21] = 0.0f; (*mtx)[M22] = 1.0f; (*mtx)[M23] = 0.0f;
		(*mtx)[M30] = 0.0f; (*mtx)[M31] = 0.0f; (*mtx)[M32] = 0.0f; (*mtx)[M33] = 1.0f;
	}

	inline MATRIX4* MtxCopy(MATRIX4 *mtxOut, const MATRIX4 *mtxIn)
	{
		(*mtxOut)[M00] = (*mtxIn)[M00]; (*mtxOut)[M01] = (*mtxIn)[M01]; (*mtxOut)[M02] = (*mtxIn)[M02]; (*mtxOut)[M03] = (*mtxIn)[M03];
		(*mtxOut)[M10] = (*mtxIn)[M10]; (*mtxOut)[M11] = (*mtxIn)[M11]; (*mtxOut)[M12] = (*mtxIn)[M12]; (*mtxOut)[M13] = (*mtxIn)[M13];
		(*mtxOut)[M20] = (*mtxIn)[M20]; (*mtxOut)[M21] = (*mtxIn)[M21]; (*mtxOut)[M22] = (*mtxIn)[M22]; (*mtxOut)[M23] = (*mtxIn)[M23];
		(*mtxOut)[M30] = (*mtxIn)[M30]; (*mtxOut)[M31] = (*mtxIn)[M31]; (*mtxOut)[M32] = (*mtxIn)[M32]; (*mtxOut)[M33] = (*mtxIn)[M33];

		return mtxOut;
	}

	inline MATRIX4* MtxSet(MATRIX4 *mtxOut,
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		(*mtxOut)[M00] = m00; (*mtxOut)[M01] = m01; (*mtxOut)[M02] = m02; (*mtxOut)[M03] = m03;
		(*mtxOut)[M10] = m10; (*mtxOut)[M11] = m11; (*mtxOut)[M12] = m12; (*mtxOut)[M13] = m13;
		(*mtxOut)[M20] = m20; (*mtxOut)[M21] = m21; (*mtxOut)[M22] = m22; (*mtxOut)[M23] = m23;
		(*mtxOut)[M30] = m30; (*mtxOut)[M31] = m31; (*mtxOut)[M32] = m32; (*mtxOut)[M33] = m33;

		return mtxOut;
	}

	inline MATRIX4* MtxMul(MATRIX4 *mtxOut, const MATRIX4 *mtxIn1, const MATRIX4 *mtxIn2)
	{
#if defined _ARM_NEON_

	#if defined __LP64__

		__asm__ volatile(
			"ld1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%1] \n\t"        // M1[m0-m7] M1[m8-m15] M2[m0-m7]  M2[m8-m15]
			"ld4 {v0.4s, v1.4s, v2.4s, v3.4s},  [%2]  \n\t"        // M2[m0-m15]


			"fmul v12.4s, v8.4s, v0.s[0]     \n\t"                 // DST->M[m0-m3] = M1[m0-m3] * M2[m0]
			"fmul v13.4s, v8.4s, v0.s[1]     \n\t"                 // DST->M[m4-m7] = M1[m4-m7] * M2[m4]
			"fmul v14.4s, v8.4s, v0.s[2]     \n\t"                 // DST->M[m8-m11] = M1[m8-m11] * M2[m8]
			"fmul v15.4s, v8.4s, v0.s[3]     \n\t"                 // DST->M[m12-m15] = M1[m12-m15] * M2[m12]

			"fmla v12.4s, v9.4s, v1.s[0]     \n\t"                 // DST->M[m0-m3] += M1[m0-m3] * M2[m1]
			"fmla v13.4s, v9.4s, v1.s[1]     \n\t"                 // DST->M[m4-m7] += M1[m4-m7] * M2[m5]
			"fmla v14.4s, v9.4s, v1.s[2]     \n\t"                 // DST->M[m8-m11] += M1[m8-m11] * M2[m9]
			"fmla v15.4s, v9.4s, v1.s[3]     \n\t"                 // DST->M[m12-m15] += M1[m12-m15] * M2[m13]

			"fmla v12.4s, v10.4s, v2.s[0]    \n\t"                 // DST->M[m0-m3] += M1[m0-m3] * M2[m2]
			"fmla v13.4s, v10.4s, v2.s[1]    \n\t"                 // DST->M[m4-m7] += M1[m4-m7] * M2[m6]
			"fmla v14.4s, v10.4s, v2.s[2]    \n\t"                 // DST->M[m8-m11] += M1[m8-m11] * M2[m10]
			"fmla v15.4s, v10.4s, v2.s[3]    \n\t"                 // DST->M[m12-m15] += M1[m12-m15] * M2[m14]

			"fmla v12.4s, v11.4s, v3.s[0]    \n\t"                 // DST->M[m0-m3] += M1[m0-m3] * M2[m3]
			"fmla v13.4s, v11.4s, v3.s[1]    \n\t"                 // DST->M[m4-m7] += M1[m4-m7] * M2[m7]
			"fmla v14.4s, v11.4s, v3.s[2]    \n\t"                 // DST->M[m8-m11] += M1[m8-m11] * M2[m11]
			"fmla v15.4s, v11.4s, v3.s[3]    \n\t"                 // DST->M[m12-m15] += M1[m12-m15] * M2[m15]

			"st1 {v12.4s, v13.4s, v14.4s, v15.4s}, [%0]  \n\t"     // DST->M[m0-m7]// DST->M[m8-m15]

			: // output
			: "r" (mtxOut), "r" (mtxIn1), "r" (mtxIn2)             // input - note *value* of pointer doesn't change
			: "memory", "v0", "v1", "v2", "v3", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
		);

	#else

		__asm__ volatile (
			"vld1.32 {d16 - d19}, [%1]!      \n\t"                 // M1[m0-m7]
			"vld1.32 {d20 - d23}, [%1]       \n\t"                 // M1[m8-m15]
			"vld1.32 {d0 - d3}, [%2]!        \n\t"                 // M2[m0-m7]
			"vld1.32 {d4 - d7}, [%2]         \n\t"                 // M2[m8-m15]

			"vmul.f32 q12, q8, d0[0]         \n\t"                 // DST->M[m0-m3] = M1[m0-m3] * M2[m0]
			"vmul.f32 q13, q8, d2[0]         \n\t"                 // DST->M[m4-m7] = M1[m4-m7] * M2[m4]
			"vmul.f32 q14, q8, d4[0]         \n\t"                 // DST->M[m8-m11] = M1[m8-m11] * M2[m8]
			"vmul.f32 q15, q8, d6[0]         \n\t"                 // DST->M[m12-m15] = M1[m12-m15] * M2[m12]

			"vmla.f32 q12, q9, d0[1]         \n\t"                 // DST->M[m0-m3] += M1[m0-m3] * M2[m1]
			"vmla.f32 q13, q9, d2[1]         \n\t"                 // DST->M[m4-m7] += M1[m4-m7] * M2[m5]
			"vmla.f32 q14, q9, d4[1]         \n\t"                 // DST->M[m8-m11] += M1[m8-m11] * M2[m9]
			"vmla.f32 q15, q9, d6[1]         \n\t"                 // DST->M[m12-m15] += M1[m12-m15] * M2[m13]

			"vmla.f32 q12, q10, d1[0]        \n\t"                 // DST->M[m0-m3] += M1[m0-m3] * M2[m2]
			"vmla.f32 q13, q10, d3[0]        \n\t"                 // DST->M[m4-m7] += M1[m4-m7] * M2[m6]
			"vmla.f32 q14, q10, d5[0]        \n\t"                 // DST->M[m8-m11] += M1[m8-m11] * M2[m10]
			"vmla.f32 q15, q10, d7[0]        \n\t"                 // DST->M[m12-m15] += M1[m12-m15] * M2[m14]

			"vmla.f32 q12, q11, d1[1]        \n\t"                 // DST->M[m0-m3] += M1[m0-m3] * M2[m3]
			"vmla.f32 q13, q11, d3[1]        \n\t"                 // DST->M[m4-m7] += M1[m4-m7] * M2[m7]
			"vmla.f32 q14, q11, d5[1]        \n\t"                 // DST->M[m8-m11] += M1[m8-m11] * M2[m11]
			"vmla.f32 q15, q11, d7[1]        \n\t"                 // DST->M[m12-m15] += M1[m12-m15] * M2[m15]

			"vst1.32 {d24 - d27}, [%0]!      \n\t"                 // DST->M[m0-m7]
			"vst1.32 {d28 - d31}, [%0]       \n\t"                 // DST->M[m8-m15]

			: // output
			: "r" (mtxOut), "r" (mtxIn1), "r" (mtxIn2)             // input - note *value* of pointer doesn't change
			: "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
		);

	#endif

#else

		(*mtxOut)[M00] = (*mtxIn1)[M00] * (*mtxIn2)[M00] + (*mtxIn1)[M01] * (*mtxIn2)[M10] + (*mtxIn1)[M02] * (*mtxIn2)[M20] + (*mtxIn1)[M03] * (*mtxIn2)[M30];
		(*mtxOut)[M01] = (*mtxIn1)[M00] * (*mtxIn2)[M01] + (*mtxIn1)[M01] * (*mtxIn2)[M11] + (*mtxIn1)[M02] * (*mtxIn2)[M21] + (*mtxIn1)[M03] * (*mtxIn2)[M31];
		(*mtxOut)[M02] = (*mtxIn1)[M00] * (*mtxIn2)[M02] + (*mtxIn1)[M01] * (*mtxIn2)[M12] + (*mtxIn1)[M02] * (*mtxIn2)[M22] + (*mtxIn1)[M03] * (*mtxIn2)[M32];
		(*mtxOut)[M03] = (*mtxIn1)[M00] * (*mtxIn2)[M03] + (*mtxIn1)[M01] * (*mtxIn2)[M13] + (*mtxIn1)[M02] * (*mtxIn2)[M23] + (*mtxIn1)[M03] * (*mtxIn2)[M33];

		(*mtxOut)[M10] = (*mtxIn1)[M10] * (*mtxIn2)[M00] + (*mtxIn1)[M11] * (*mtxIn2)[M10] + (*mtxIn1)[M12] * (*mtxIn2)[M20] + (*mtxIn1)[M13] * (*mtxIn2)[M30];
		(*mtxOut)[M11] = (*mtxIn1)[M10] * (*mtxIn2)[M01] + (*mtxIn1)[M11] * (*mtxIn2)[M11] + (*mtxIn1)[M12] * (*mtxIn2)[M21] + (*mtxIn1)[M13] * (*mtxIn2)[M31];
		(*mtxOut)[M12] = (*mtxIn1)[M10] * (*mtxIn2)[M02] + (*mtxIn1)[M11] * (*mtxIn2)[M12] + (*mtxIn1)[M12] * (*mtxIn2)[M22] + (*mtxIn1)[M13] * (*mtxIn2)[M32];
		(*mtxOut)[M13] = (*mtxIn1)[M10] * (*mtxIn2)[M03] + (*mtxIn1)[M11] * (*mtxIn2)[M13] + (*mtxIn1)[M12] * (*mtxIn2)[M23] + (*mtxIn1)[M13] * (*mtxIn2)[M33];

		(*mtxOut)[M20] = (*mtxIn1)[M20] * (*mtxIn2)[M00] + (*mtxIn1)[M21] * (*mtxIn2)[M10] + (*mtxIn1)[M22] * (*mtxIn2)[M20] + (*mtxIn1)[M23] * (*mtxIn2)[M30];
		(*mtxOut)[M21] = (*mtxIn1)[M20] * (*mtxIn2)[M01] + (*mtxIn1)[M21] * (*mtxIn2)[M11] + (*mtxIn1)[M22] * (*mtxIn2)[M21] + (*mtxIn1)[M23] * (*mtxIn2)[M31];
		(*mtxOut)[M22] = (*mtxIn1)[M20] * (*mtxIn2)[M02] + (*mtxIn1)[M21] * (*mtxIn2)[M12] + (*mtxIn1)[M22] * (*mtxIn2)[M22] + (*mtxIn1)[M23] * (*mtxIn2)[M32];
		(*mtxOut)[M23] = (*mtxIn1)[M20] * (*mtxIn2)[M03] + (*mtxIn1)[M21] * (*mtxIn2)[M13] + (*mtxIn1)[M22] * (*mtxIn2)[M23] + (*mtxIn1)[M23] * (*mtxIn2)[M33];

		(*mtxOut)[M30] = (*mtxIn1)[M30] * (*mtxIn2)[M00] + (*mtxIn1)[M31] * (*mtxIn2)[M10] + (*mtxIn1)[M32] * (*mtxIn2)[M20] + (*mtxIn1)[M33] * (*mtxIn2)[M30];
		(*mtxOut)[M31] = (*mtxIn1)[M30] * (*mtxIn2)[M01] + (*mtxIn1)[M31] * (*mtxIn2)[M11] + (*mtxIn1)[M32] * (*mtxIn2)[M21] + (*mtxIn1)[M33] * (*mtxIn2)[M31];
		(*mtxOut)[M32] = (*mtxIn1)[M30] * (*mtxIn2)[M02] + (*mtxIn1)[M31] * (*mtxIn2)[M12] + (*mtxIn1)[M32] * (*mtxIn2)[M22] + (*mtxIn1)[M33] * (*mtxIn2)[M32];
		(*mtxOut)[M33] = (*mtxIn1)[M30] * (*mtxIn2)[M03] + (*mtxIn1)[M31] * (*mtxIn2)[M13] + (*mtxIn1)[M32] * (*mtxIn2)[M23] + (*mtxIn1)[M33] * (*mtxIn2)[M33];

#endif

		return mtxOut;
	}

	inline MATRIX4* MtxInverse(MATRIX4 *mtxOut, const MATRIX4 *mtxIn)
	{
		float det;
		float invDet;
		float a, b, c;

		a = (*mtxIn)[M11] * (*mtxIn)[M22] - (*mtxIn)[M12] * (*mtxIn)[M21];
		b = (*mtxIn)[M21] * (*mtxIn)[M02] - (*mtxIn)[M01] * (*mtxIn)[M22];
		c = (*mtxIn)[M01] * (*mtxIn)[M12] - (*mtxIn)[M02] * (*mtxIn)[M11];

		det = (*mtxIn)[M00] * a + (*mtxIn)[M10] * b + (*mtxIn)[M20] * c;

		if (FastFAbs(det) < EPSILON_E3) {
			return NULL;
		}

		invDet = 1.0f / det;

		(*mtxOut)[M00] =  ((*mtxIn)[M11] * (*mtxIn)[M22] - (*mtxIn)[M12] * (*mtxIn)[M21])*invDet;
		(*mtxOut)[M01] = -((*mtxIn)[M01] * (*mtxIn)[M22] - (*mtxIn)[M02] * (*mtxIn)[M21])*invDet;
		(*mtxOut)[M02] =  ((*mtxIn)[M01] * (*mtxIn)[M12] - (*mtxIn)[M02] * (*mtxIn)[M11])*invDet;
		(*mtxOut)[M03] =  0.0f;

		(*mtxOut)[M10] = -((*mtxIn)[M10] * (*mtxIn)[M22] - (*mtxIn)[M12] * (*mtxIn)[M20])*invDet;
		(*mtxOut)[M11] =  ((*mtxIn)[M00] * (*mtxIn)[M22] - (*mtxIn)[M02] * (*mtxIn)[M20])*invDet;
		(*mtxOut)[M12] = -((*mtxIn)[M00] * (*mtxIn)[M12] - (*mtxIn)[M02] * (*mtxIn)[M10])*invDet;
		(*mtxOut)[M13] =  0.0f;

		(*mtxOut)[M20] =  ((*mtxIn)[M10] * (*mtxIn)[M21] - (*mtxIn)[M11] * (*mtxIn)[M20])*invDet;
		(*mtxOut)[M21] = -((*mtxIn)[M00] * (*mtxIn)[M21] - (*mtxIn)[M01] * (*mtxIn)[M20])*invDet;
		(*mtxOut)[M22] =  ((*mtxIn)[M00] * (*mtxIn)[M11] - (*mtxIn)[M01] * (*mtxIn)[M10])*invDet;
		(*mtxOut)[M23] =  0.0f;

		(*mtxOut)[M30] = -((*mtxOut)[M00] * (*mtxIn)[M30] + (*mtxOut)[M10] * (*mtxIn)[M31] + (*mtxOut)[M20] * (*mtxIn)[M32]);
		(*mtxOut)[M31] = -((*mtxOut)[M01] * (*mtxIn)[M30] + (*mtxOut)[M11] * (*mtxIn)[M31] + (*mtxOut)[M21] * (*mtxIn)[M32]);
		(*mtxOut)[M32] = -((*mtxOut)[M02] * (*mtxIn)[M30] + (*mtxOut)[M12] * (*mtxIn)[M31] + (*mtxOut)[M22] * (*mtxIn)[M32]);
		(*mtxOut)[M33] =  1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxTranspose(MATRIX4 *mtxOut, const MATRIX4 *mtxIn)
	{
#if defined _ARM_NEON_

	#if defined __LP64__

		__asm__ volatile (
			"ld4 {v0.4s, v1.4s, v2.4s, v3.4s}, [%1]         \n\t" // DST->M[m0, m4, m8, m12] = M[m0-m3] 
			"st1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%0]         \n\t" // DST->M[m1, m5, m9, m12] = M[m4-m7] 

			: // output
			: "r"(mtxOut), "r"(mtxIn)
			: "v0", "v1", "v2", "v3", "memory"
		);

	#else

		__asm__ volatile(
			"vld4.32 {d0[0], d2[0], d4[0], d6[0]}, [%1]!    \n\t" // DST->M[m0, m4, m8, m12] = M[m0-m3]
			"vld4.32 {d0[1], d2[1], d4[1], d6[1]}, [%1]!    \n\t" // DST->M[m1, m5, m9, m12] = M[m4-m7]
			"vld4.32 {d1[0], d3[0], d5[0], d7[0]}, [%1]!    \n\t" // DST->M[m2, m6, m10, m12] = M[m8-m11]
			"vld4.32 {d1[1], d3[1], d5[1], d7[1]}, [%1]     \n\t" // DST->M[m3, m7, m11, m12] = M[m12-m15]

			"vst1.32 {q0-q1}, [%0]!                         \n\t" // DST->M[m0-m7]
			"vst1.32 {q2-q3}, [%0]                          \n\t" // DST->M[m8-m15]

			: // output
			: "r"(mtxOut), "r"(mtxIn)
			: "q0", "q1", "q2", "q3", "memory"
		);

	#endif

#else

		(*mtxOut)[M00] = (*mtxIn)[M00]; (*mtxOut)[M01] = (*mtxIn)[M10]; (*mtxOut)[M02] = (*mtxIn)[M20]; (*mtxOut)[M03] = (*mtxIn)[M03];
		(*mtxOut)[M10] = (*mtxIn)[M01]; (*mtxOut)[M11] = (*mtxIn)[M11]; (*mtxOut)[M12] = (*mtxIn)[M21]; (*mtxOut)[M13] = (*mtxIn)[M13];
		(*mtxOut)[M20] = (*mtxIn)[M02]; (*mtxOut)[M21] = (*mtxIn)[M12]; (*mtxOut)[M22] = (*mtxIn)[M22]; (*mtxOut)[M23] = (*mtxIn)[M23];
		(*mtxOut)[M30] = (*mtxIn)[M30]; (*mtxOut)[M31] = (*mtxIn)[M31]; (*mtxOut)[M32] = (*mtxIn)[M32]; (*mtxOut)[M33] = (*mtxIn)[M33];

#endif

		return mtxOut;
	}

	inline MATRIX4* MtxReflect(MATRIX4 *mtxOut, const VEC3 *vecInPoint, const VEC3 *vecInNormal)
	{
		float xy, xz, yz;
		float x2, y2, z2;
		float dot, dotx, doty, dotz;

		dot = Vec3Dot(vecInPoint, vecInNormal)*2.0f;
		dotx = (*vecInNormal)[0] * dot;
		doty = (*vecInNormal)[1] * dot;
		dotz = (*vecInNormal)[2] * dot;

		xy = -(*vecInNormal)[0] * (*vecInNormal)[1] * 2.0f;
		xz = -(*vecInNormal)[0] * (*vecInNormal)[2] * 2.0f;
		yz = -(*vecInNormal)[1] * (*vecInNormal)[2] * 2.0f;

		x2 = 1.0f - (*vecInNormal)[0] * (*vecInNormal)[0] * 2.0f;
		y2 = 1.0f - (*vecInNormal)[1] * (*vecInNormal)[1] * 2.0f;
		z2 = 1.0f - (*vecInNormal)[2] * (*vecInNormal)[2] * 2.0f;

		(*mtxOut)[M00] = x2;   (*mtxOut)[M01] = xy;   (*mtxOut)[M02] = xz;   (*mtxOut)[M03] = 0.0f;
		(*mtxOut)[M10] = xy;   (*mtxOut)[M11] = y2;   (*mtxOut)[M12] = yz;   (*mtxOut)[M13] = 0.0f;
		(*mtxOut)[M20] = xz;   (*mtxOut)[M21] = yz;   (*mtxOut)[M22] = z2;   (*mtxOut)[M23] = 0.0f;
		(*mtxOut)[M30] = dotx; (*mtxOut)[M31] = doty; (*mtxOut)[M32] = dotz; (*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxDefLookAt(MATRIX4 *mtxOut, const VEC3 *posIn, const VEC3 *upIn, const VEC3 *targetIn)
	{
		//
		//            point:up vec:V  y
		//                     V=NxU |
		//                           |
		//                     z     |
		//  point:target vec:N ______| pos
		//                          /
		//                         / vec:U
		//                      x /  U=VxN
		//
		//    n = normal(targetIn - posIn)
		//    u = normal(cross(v, n))
		//    v = cross(n, u)
		//
		//    u.x           v.x           n.x          0
		//    u.y           v.y           n.y          0
		//    u.z           v.z           n.z          0
		//   -dot(u, eye)  -dot(v, eye)  -dot(n, eye)  l
		//

		VEC3 u, v, n;

		Vec3Set(&v, (*upIn)[0], (*upIn)[1], (*upIn)[2]);
		Vec3Set(&n, (*posIn)[0] - (*targetIn)[0], (*posIn)[1] - (*targetIn)[1], (*posIn)[2] - (*targetIn)[2]);
		Vec3Normalize(&n);

		Vec3Cross(&u, &v, &n);
		Vec3Normalize(&u);

		Vec3Cross(&v, &n, &u);
		Vec3Normalize(&v);

		float tx = Vec3Dot(posIn, &u);
		float ty = Vec3Dot(posIn, &v);
		float tz = Vec3Dot(posIn, &n);

		MtxSet(mtxOut,
			u[0], v[0], n[0], 0.0f,
			u[1], v[1], n[1], 0.0f,
			u[2], v[2], n[2], 0.0f,
			-tx,  -ty,  -tz,  1.0f);

		return mtxOut;
	}

	inline MATRIX4* MtxDefPerspective(MATRIX4 *mtxOut, float fovy, float aspect, float zNear, float zFar)
	{
		//
		// 视锥示意图如下: 
		//                   z
		//                 /|\
		//   _\_____________|_____________/_ 
		//     \            |            /  远截面
		//      \           |           /
		//       \          |          /
		// 右截面 \         |         / 左截面
		//        _\________|________/_ 
		//          \       |       /   近截面
		//           \w/2=1 | w/2=1/ w = 2
		//           _\_____|_____/_
		//             \    |    /|
		//              \   |   / |view dis = w/2/tan(theta/2)
		//               \theta/  |         = 1/tan(theta/2)
		//                \ | /   |
		//   ______________\|/____|___________\ x
		//                                    /
		//
		//    xScale     0          0               0
		//    0        yScale       0               0
		//    0          0       -(zf+zn)/(zf-zn)  -1
		//    0          0     -2*(zf*zn)/(zf-zn)   0
		//
		//    where:
		//    yScale = 1.0f / tan(fovy/2)
		//    xScale = yScale / aspect ratio
		//

		float d = 1.0f / FastTan(DEG2RAD(fovy)*0.5f);
		float daspect = d / aspect;
		float invDis = 1.0f / (zFar - zNear);

		MtxSet(mtxOut,
			daspect, 0.0f,  0.0f,                           0.0f,
			0.0f,    d,     0.0f,                           0.0f,
			0.0f,    0.0f, -(zFar + zNear) * invDis,       -1.0f,
			0.0f,    0.0f, -2.0f * (zFar * zNear) * invDis, 0.0f);

		return mtxOut;
	}

	inline MATRIX4* MtxDefFrustum(MATRIX4 *mtxOut, float t, float b, float l, float r, float n, float f)
	{
		MtxSet(mtxOut,
			(n + n) / (r - l), 0.0f,              0.0f,                    0.0f,
			0.0f,              (n + n) / (t - b), 0.0f,                    0.0f,
			(l + r) / (r - l), (t + b) / (t - b), (f + n) / (n - f),      -1.0f,
			0.0f,              0.0f,              2.0f * f * n / (n - f),  0.0f);

		return mtxOut;
	}

	inline MATRIX4* MtxDefOrtho(MATRIX4 *mtxOut, float t, float b, float l, float r, float n, float f)
	{
		MtxSet(mtxOut,
			2.0f / (r - l),    0.0f,              0.0f,              0.0f,
			0.0f,              2.0f / (t - b),    0.0f,              0.0f,
			0.0f,              0.0f,              2.0f / (n - f),    0.0f,
			(l + r) / (l - r), (b + t) / (b - t), (n + f) / (n - f), 1.0f);

		return mtxOut;
	}

	inline MATRIX4* MtxDefViewport(MATRIX4 *mtxOut, float x, float y, float width, float height)
	{
		MtxSet(mtxOut,
			width / 2.0f,     0.0f,              0.0f, 0.0f,
			0.0f,            -height / 2.0f,     0.0f, 0.0f,
			0.0f,             0.0f,              1.0f, 0.0f,
			width / 2.0f + x, height / 2.0f + y, 0.0f, 1.0f);

		return mtxOut;
	}

	inline MATRIX4* MtxDefTranslate(MATRIX4 *mtxOut, float x, float y, float z)
	{
		MtxIdentity(mtxOut);

		(*mtxOut)[M30] = x;
		(*mtxOut)[M31] = y;
		(*mtxOut)[M32] = z;

		return mtxOut;
	}

	inline MATRIX4* MtxDefScale(MATRIX4 *mtxOut, float scaleX, float scaleY, float scaleZ)
	{
		MtxIdentity(mtxOut);

		(*mtxOut)[M00] = scaleX;
		(*mtxOut)[M11] = scaleY;
		(*mtxOut)[M22] = scaleZ;
		(*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxDefRotateLookAt(MATRIX4 *mtxOut, const VEC3 *directionIn, const VEC3 *upIn)
	{
		VEC3 u, v, n;

		Vec3Set(&v, (*upIn)[0], (*upIn)[1], (*upIn)[2]);
		Vec3Set(&n, (*directionIn)[0], (*directionIn)[1], (*directionIn)[2]);
		Vec3Normalize(&n);

		Vec3Cross(&u, &v, &n);
		Vec3Normalize(&u);

		Vec3Cross(&v, &n, &u);
		Vec3Normalize(&v);

		MtxSet(mtxOut,
			u[0], u[1], u[2], 0.0f,
			v[0], v[1], v[2], 0.0f,
			n[0], n[1], n[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return mtxOut;
	}

	inline MATRIX4* MtxDefRotateAxisAngle(MATRIX4 *mtxOut, const VEC3 *vecInAxis, float rad)
	{
		VEC3 axis;
		float s, c, t;
		float x2, y2, z2;
		float xy, xz, yz;
		float sx, sy, sz;
		float txy, txz, tyz;
		float tx2, ty2, tz2;

		s = FastSin(rad);
		c = FastCos(rad);
		t = 1.0f - c;

		Vec3Copy(&axis, vecInAxis);
		Vec3Normalize(&axis);

		x2 = axis[0] * axis[0];
		y2 = axis[1] * axis[1];
		z2 = axis[2] * axis[2];

		xy = axis[0] * axis[1];
		xz = axis[0] * axis[2];
		yz = axis[1] * axis[2];

		sx = s*axis[0];
		sy = s*axis[1];
		sz = s*axis[2];

		txy = t*xy;
		txz = t*xz;
		tyz = t*yz;

		tx2 = t*x2;
		ty2 = t*y2;
		tz2 = t*z2;

		(*mtxOut)[M00] = tx2 + c;  (*mtxOut)[M01] = txy + sz; (*mtxOut)[M02] = txz - sy; (*mtxOut)[M03] = 0.0f;
		(*mtxOut)[M10] = txy - sz; (*mtxOut)[M11] = ty2 + c;  (*mtxOut)[M12] = tyz + sx; (*mtxOut)[M13] = 0.0f;
		(*mtxOut)[M20] = txz + sy; (*mtxOut)[M21] = tyz - sx; (*mtxOut)[M22] = tz2 + c;  (*mtxOut)[M23] = 0.0f;
		(*mtxOut)[M30] = 0.0f;     (*mtxOut)[M31] = 0.0f;     (*mtxOut)[M32] = 0.0f;     (*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxDefRotateAngleXYZ(MATRIX4 *mtxOut, float radX, float radY, float radZ)
	{
		float sx, sy, sz;
		float cx, cy, cz;
		float sxsz, sysz, szcy, sxcz, sycz;
		float cxcy, cycz, cxsy, cxsz, cxcz;
		float sxszsy, sxszcy, sxczsy, sxczcy;

		sx = FastSin(radX);
		sy = FastSin(radY);
		sz = FastSin(radZ);

		cx = FastCos(radX);
		cy = FastCos(radY);
		cz = FastCos(radZ);

		sxsz = sx * sz;
		sysz = sy * sz;
		szcy = sz * cy;
		sxcz = sx * cz;
		sycz = sy * cz;

		cxcy = cx * cy;
		cycz = cy * cz;
		cxsy = cx * sy;
		cxsz = cx * sz;
		cxcz = cx * cz;

		sxszsy = sxsz * sy;
		sxszcy = sxsz * cy;
		sxczsy = sxcz * sy;
		sxczcy = sxcz * cy;

		(*mtxOut)[M00] = cycz + sxszsy;  (*mtxOut)[M01] = cxsz;    (*mtxOut)[M02] = -sycz + sxszcy; (*mtxOut)[M03] = 0.0f;
		(*mtxOut)[M10] = -szcy + sxczsy; (*mtxOut)[M11] = cxcz;    (*mtxOut)[M12] = sysz + sxczcy;  (*mtxOut)[M13] = 0.0f;
		(*mtxOut)[M20] = cxsy;           (*mtxOut)[M21] = -sx;     (*mtxOut)[M22] = cxcy;           (*mtxOut)[M23] = 0.0f;
		(*mtxOut)[M30] = 0.0f;           (*mtxOut)[M31] = 0.0f;    (*mtxOut)[M32] = 0.0f;           (*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxDefRotateAngleX(MATRIX4 *mtxOut, float rad)
	{
		//
		//      | 1   0      0       0 |
		// Rx = | 0   cos A  sin A   0 |
		//      | 0  -sin A  cos A   0 |
		//      | 0   0      0       1 |
		//

		float s, c;

		s = FastSin(rad);
		c = FastCos(rad);

		(*mtxOut)[M00] = 1.0f; (*mtxOut)[M01] = 0.0f; (*mtxOut)[M02] = 0.0f; (*mtxOut)[M03] = 0.0f;
		(*mtxOut)[M10] = 0.0f; (*mtxOut)[M11] = c;    (*mtxOut)[M12] = s;    (*mtxOut)[M13] = 0.0f;
		(*mtxOut)[M20] = 0.0f; (*mtxOut)[M21] = -s;   (*mtxOut)[M22] = c;    (*mtxOut)[M23] = 0.0f;
		(*mtxOut)[M30] = 0.0f; (*mtxOut)[M31] = 0.0f; (*mtxOut)[M32] = 0.0f; (*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxDefRotateAngleY(MATRIX4 *mtxOut, float rad)
	{
		//
		//      | cos A   0   -sin A  0 |
		// Ry = | 0       1    0      0 |
		//      | sin A   0    cos A  0 |
		//      | 0       0    0      1 |
		//

		float s, c;

		s = FastSin(rad);
		c = FastCos(rad);

		(*mtxOut)[M00] = c;    (*mtxOut)[M01] = 0.0f; (*mtxOut)[M02] = -s;   (*mtxOut)[M03] = 0.0f;
		(*mtxOut)[M10] = 0.0f; (*mtxOut)[M11] = 1.0f; (*mtxOut)[M12] = 0.0f; (*mtxOut)[M13] = 0.0f;
		(*mtxOut)[M20] = s;    (*mtxOut)[M21] = 0.0f; (*mtxOut)[M22] = c;    (*mtxOut)[M23] = 0.0f;
		(*mtxOut)[M30] = 0.0f; (*mtxOut)[M31] = 0.0f; (*mtxOut)[M32] = 0.0f; (*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxDefRotateAngleZ(MATRIX4 *mtxOut, float rad)
	{
		//
		//      |  cos A   sin A    0  0 |
		// Rz = | -sin A   cos A    0  0 |
		//      |  0       0        1  0 |
		//      |  0       0        0  1 |
		//

		float s, c;

		s = FastSin(rad);
		c = FastCos(rad);

		(*mtxOut)[M00] = c;    (*mtxOut)[M01] = s;    (*mtxOut)[M02] = 0.0f; (*mtxOut)[M03] = 0.0f;
		(*mtxOut)[M10] = -s;   (*mtxOut)[M11] = c;    (*mtxOut)[M12] = 0.0f; (*mtxOut)[M13] = 0.0f;
		(*mtxOut)[M20] = 0.0f; (*mtxOut)[M21] = 0.0f; (*mtxOut)[M22] = 1.0f; (*mtxOut)[M23] = 0.0f;
		(*mtxOut)[M30] = 0.0f; (*mtxOut)[M31] = 0.0f; (*mtxOut)[M32] = 0.0f; (*mtxOut)[M33] = 1.0f;

		return mtxOut;
	}

	inline MATRIX4* MtxTranslate(MATRIX4 *mtxOut, float x, float y, float z)
	{
		(*mtxOut)[M30] += (*mtxOut)[M00] * x + (*mtxOut)[M10] * y + (*mtxOut)[M20] * z;
		(*mtxOut)[M31] += (*mtxOut)[M01] * x + (*mtxOut)[M11] * y + (*mtxOut)[M21] * z;
		(*mtxOut)[M32] += (*mtxOut)[M02] * x + (*mtxOut)[M12] * y + (*mtxOut)[M22] * z;

		return mtxOut;
	}

	inline MATRIX4* MtxScale(MATRIX4 *mtxOut, float scaleX, float scaleY, float scaleZ)
	{
		MATRIX4 mtxScale, matrix;

		MtxDefScale(&mtxScale, scaleX, scaleY, scaleZ);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxScale, &matrix);

		return mtxOut;
	}

	inline MATRIX4* QuatToMtxRotation(MATRIX4 *mtxOut, const QUAT *quatIn);
	inline MATRIX4* MtxRotateQuat(MATRIX4 *mtxOut, const QUAT *quatIn)
	{
		MATRIX4 mtxRotate, matrix;

		QuatToMtxRotation(&mtxRotate, quatIn);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxRotate, &matrix);

		return mtxOut;
	}

	inline MATRIX4* MtxRotateAxisAngle(MATRIX4 *mtxOut, const VEC3 *vecInAxis, float rad)
	{
		MATRIX4 mtxRotate, matrix;

		MtxDefRotateAxisAngle(&mtxRotate, vecInAxis, rad);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxRotate, &matrix);

		return mtxOut;
	}

	inline MATRIX4* MtxRotateAngleXYZ(MATRIX4 *mtxOut, float radX, float radY, float radZ)
	{
		MATRIX4 mtxRotate, matrix;

		MtxDefRotateAngleXYZ(&mtxRotate, radX, radY, radZ);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxRotate, &matrix);

		return mtxOut;
	}

	inline MATRIX4* MtxRotateAngleX(MATRIX4 *mtxOut, float rad)
	{
		MATRIX4 mtxRotate, matrix;

		MtxDefRotateAngleX(&mtxRotate, rad);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxRotate, &matrix);

		return mtxOut;
	}

	inline MATRIX4* MtxRotateAngleY(MATRIX4 *mtxOut, float rad)
	{
		MATRIX4 mtxRotate, matrix;

		MtxDefRotateAngleY(&mtxRotate, rad);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxRotate, &matrix);

		return mtxOut;
	}

	inline MATRIX4* MtxRotateAngleZ(MATRIX4 *mtxOut, float rad)
	{
		MATRIX4 mtxRotate, matrix;

		MtxDefRotateAngleZ(&mtxRotate, rad);
		MtxCopy(&matrix, mtxOut);
		MtxMul(mtxOut, &mtxRotate, &matrix);

		return mtxOut;
	}

	inline VEC2* Vec2MulMtx4x4(VEC2 *vecOut, const VEC2 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*mtxIn)[M30];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*mtxIn)[M31];

		return vecOut;
	}

	inline VEC3* Vec3MulMtx4x4(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20] + (*mtxIn)[M30];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21] + (*mtxIn)[M31];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22] + (*mtxIn)[M32];

		return vecOut;
	}

	inline VEC3* Vec3MulMtx3x3(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22];

		return vecOut;
	}

	inline VEC4* Vec4MulMtx4x4(VEC4 *vecOut, const VEC4 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20] + (*vecIn)[3] * (*mtxIn)[M30];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21] + (*vecIn)[3] * (*mtxIn)[M31];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22] + (*vecIn)[3] * (*mtxIn)[M32];
		(*vecOut)[3] = (*vecIn)[0] * (*mtxIn)[M03] + (*vecIn)[1] * (*mtxIn)[M13] + (*vecIn)[2] * (*mtxIn)[M23] + (*vecIn)[3] * (*mtxIn)[M33];

		return vecOut;
	}

	inline VEC3* Mtx4x4MulVec3(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M01] + (*vecIn)[2] * (*mtxIn)[M02] + (*mtxIn)[M03];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M10] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M12] + (*mtxIn)[M13];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M20] + (*vecIn)[1] * (*mtxIn)[M21] + (*vecIn)[2] * (*mtxIn)[M22] + (*mtxIn)[M23];

		return vecOut;
	}

	inline VEC3* Mtx3x3MulVec3(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M01] + (*vecIn)[2] * (*mtxIn)[M02];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M10] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M12];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M20] + (*vecIn)[1] * (*mtxIn)[M21] + (*vecIn)[2] * (*mtxIn)[M22];

		return vecOut;
	}

	inline VEC4* Mtx4x4MulVec4(VEC4 *vecOut, const VEC4 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M01] + (*vecIn)[2] * (*mtxIn)[M02] + (*vecIn)[3] * (*mtxIn)[M03];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M10] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M12] + (*vecIn)[3] * (*mtxIn)[M13];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M20] + (*vecIn)[1] * (*mtxIn)[M21] + (*vecIn)[2] * (*mtxIn)[M22] + (*vecIn)[3] * (*mtxIn)[M23];
		(*vecOut)[3] = (*vecIn)[0] * (*mtxIn)[M30] + (*vecIn)[1] * (*mtxIn)[M31] + (*vecIn)[2] * (*mtxIn)[M32] + (*vecIn)[3] * (*mtxIn)[M33];

		return vecOut;
	}

	inline VEC3* RotateVector(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22];

		return vecOut;
	}

	inline VEC3* RotateVectorX(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20];

		return vecOut;
	}

	inline VEC3* RotateVectorY(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21];

		return vecOut;
	}

	inline VEC3* RotateVectorZ(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22];

		return vecOut;
	}

	inline VEC3* InvRotateVector(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M01] + (*vecIn)[2] * (*mtxIn)[M02];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M10] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M12];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M20] + (*vecIn)[1] * (*mtxIn)[M21] + (*vecIn)[2] * (*mtxIn)[M22];

		return vecOut;
	}

	inline VEC3* InvRotateVectorX(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M01] + (*vecIn)[2] * (*mtxIn)[M02];

		return vecOut;
	}

	inline VEC3* InvRotateVectorY(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M10] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M12];

		return vecOut;
	}

	inline VEC3* InvRotateVectorZ(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M20] + (*vecIn)[1] * (*mtxIn)[M21] + (*vecIn)[2] * (*mtxIn)[M22];

		return vecOut;
	}

	inline VEC3* TranslateVector(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20] + (*mtxIn)[M30];
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21] + (*mtxIn)[M31];
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22] + (*mtxIn)[M32];

		return vecOut;
	}

	inline VEC3* TranslateVectorX(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[0] = (*vecIn)[0] * (*mtxIn)[M00] + (*vecIn)[1] * (*mtxIn)[M10] + (*vecIn)[2] * (*mtxIn)[M20] + (*mtxIn)[M30];

		return vecOut;
	}

	inline VEC3* TranslateVectorY(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[1] = (*vecIn)[0] * (*mtxIn)[M01] + (*vecIn)[1] * (*mtxIn)[M11] + (*vecIn)[2] * (*mtxIn)[M21] + (*mtxIn)[M31];

		return vecOut;
	}

	inline VEC3* TranslateVectorZ(VEC3 *vecOut, const VEC3 *vecIn, const MATRIX4 *mtxIn)
	{
		(*vecOut)[2] = (*vecIn)[0] * (*mtxIn)[M02] + (*vecIn)[1] * (*mtxIn)[M12] + (*vecIn)[2] * (*mtxIn)[M22] + (*mtxIn)[M32];

		return vecOut;
	}

	inline void MtxToAngleXYZ(float *radXOut, float *radYOut, float *radZOut, const MATRIX4 *mtxIn)
	{
		if (radXOut) *radXOut = -FastASin((*mtxIn)[M21]);
		if (radYOut) *radYOut = FastATan2((*mtxIn)[M20], (*mtxIn)[M22]);
		if (radZOut) *radZOut = FastATan2((*mtxIn)[M01], (*mtxIn)[M11]);
	}

	inline VEC3* MtxToScale(VEC3 *scaleOut, const MATRIX4 *mtxIn)
	{
		VEC3 initUnitX, unitX;
		VEC3 initUnitY, unitY;
		VEC3 initUnitZ, unitZ;

		Vec3Set(&initUnitX, 1.0f, 0.0f, 0.0f);
		Vec3Set(&initUnitY, 0.0f, 1.0f, 0.0f);
		Vec3Set(&initUnitZ, 0.0f, 0.0f, 1.0f);

		Vec3MulMtx3x3(&unitX, &initUnitX, mtxIn);
		Vec3MulMtx3x3(&unitY, &initUnitY, mtxIn);
		Vec3MulMtx3x3(&unitZ, &initUnitZ, mtxIn);

		// ??? 对于负缩放无法计算
		(*scaleOut)[0] = Vec3Length(&unitX);
		(*scaleOut)[1] = Vec3Length(&unitY);
		(*scaleOut)[2] = Vec3Length(&unitZ);

		return scaleOut;
	}

	inline VEC3* MtxToTranslate(VEC3 *translateOut, const MATRIX4 *mtxIn)
	{
		VEC3 initTranslate;

		Vec3Zero(&initTranslate);
		Vec3MulMtx4x4(translateOut, &initTranslate, mtxIn);

		return translateOut;
	}

	inline float QuatNormalize(QUAT *quat);
	inline QUAT* MtxToQuat(QUAT *quatOut, const MATRIX4 *mtxIn)
	{
		float s, invs;

		if ((*mtxIn)[M00] + (*mtxIn)[M11] + (*mtxIn)[M22] > 0.0f) {
			s = FastSqrt((*mtxIn)[M00] + (*mtxIn)[M11] + (*mtxIn)[M22] + 1.0f);
			invs = 0.5f / s;

			(*quatOut)[0] = ((*mtxIn)[M12] - (*mtxIn)[M21])*invs;
			(*quatOut)[1] = ((*mtxIn)[M20] - (*mtxIn)[M02])*invs;
			(*quatOut)[2] = ((*mtxIn)[M01] - (*mtxIn)[M10])*invs;
			(*quatOut)[3] = 0.5f*s;
		}
		else {
			if ((*mtxIn)[M11] < (*mtxIn)[M00] && (*mtxIn)[M22] < (*mtxIn)[M00]) {
				s = FastSqrt(((*mtxIn)[M00] - ((*mtxIn)[M11] + (*mtxIn)[M22])) + 1.0f);
				invs = 0.5f / s;

				(*quatOut)[0] = 0.5f*s;
				(*quatOut)[1] = ((*mtxIn)[M10] + (*mtxIn)[M01])*invs;
				(*quatOut)[2] = ((*mtxIn)[M20] + (*mtxIn)[M02])*invs;
				(*quatOut)[3] = ((*mtxIn)[M12] - (*mtxIn)[M21])*invs;
			}
			else if ((*mtxIn)[M11] > (*mtxIn)[M00] && (*mtxIn)[M22] < (*mtxIn)[M11]) {
				s = FastSqrt(((*mtxIn)[M11] - ((*mtxIn)[M22] + (*mtxIn)[M00])) + 1.0f);
				invs = 0.5f / s;

				(*quatOut)[0] = ((*mtxIn)[M01] + (*mtxIn)[M10])*invs;
				(*quatOut)[1] = 0.5f*s;
				(*quatOut)[2] = ((*mtxIn)[M21] + (*mtxIn)[M12])*invs;
				(*quatOut)[3] = ((*mtxIn)[M20] - (*mtxIn)[M02])*invs;
			}
			else if (((*mtxIn)[M11] > (*mtxIn)[M00] && (*mtxIn)[M22] > (*mtxIn)[M11]) || ((*mtxIn)[M11] < (*mtxIn)[M00] && (*mtxIn)[M22] > (*mtxIn)[M00])) {
				s = FastSqrt(((*mtxIn)[M22] - ((*mtxIn)[M00] + (*mtxIn)[M11])) + 1.0f);
				invs = 0.5f / s;

				(*quatOut)[0] = ((*mtxIn)[M02] + (*mtxIn)[M20])*invs;
				(*quatOut)[1] = ((*mtxIn)[M12] + (*mtxIn)[M21])*invs;
				(*quatOut)[2] = 0.5f*s;
				(*quatOut)[3] = ((*mtxIn)[M01] - (*mtxIn)[M10])*invs;
			}
		}

		QuatNormalize(quatOut);

		return quatOut;
	}

	inline VEC3* Project(VEC3 *screenOut, const VEC3 *worldIn, const MATRIX4 *mtxWorldToCamera, const MATRIX4 *mtxProjection, int viewportX, int viewportY, int viewportWidth, int viewportHeight)
	{
		VEC4 world;
		VEC4 camera;
		VEC4 project;

		Vec4Set(&world, (*worldIn)[0], (*worldIn)[1], (*worldIn)[2], 1.0f);
		Vec4MulMtx4x4(&camera, &world, mtxWorldToCamera);
		Vec4MulMtx4x4(&project, &camera, mtxProjection);

		project[3] = -1.0f / camera[2];
		project[0] *= project[3];
		project[1] *= project[3];
		project[2] *= project[3];

		(*screenOut)[0] = (1.0f + project[0])*0.5f*viewportWidth + viewportX;
		(*screenOut)[1] = (1.0f - project[1])*0.5f*viewportHeight + viewportY;
		(*screenOut)[2] = (1.0f + project[2])*0.5f;

		return screenOut;
	}

	inline VEC3* UnProject(VEC3 *worldOut, const VEC2 *screenIn, const MATRIX4 *mtxCameraToWorld, const MATRIX4 *mtxProjection, int viewportX, int viewportY, int viewportWidth, int viewportHeight)
	{
		VEC3 camera;

		camera[0] = (((2.0f*((*screenIn)[0] - viewportX)) / viewportWidth) - 1.0f) / (*mtxProjection)[M00];
		camera[1] = -(((2.0f*((*screenIn)[1] - viewportY)) / viewportHeight) - 1.0f) / (*mtxProjection)[M11];
		camera[2] = -1.0f;

		Vec3MulMtx4x4(worldOut, &camera, mtxCameraToWorld);

		return worldOut;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 四元数运算
	//=============================================================================
	inline void QuatZero(QUAT *quat)
	{
		(*quat)[0] = 0.0f;
		(*quat)[1] = 0.0f;
		(*quat)[2] = 0.0f;
		(*quat)[3] = 1.0f;
	}

	inline QUAT* QuatSet(QUAT *quat, float x, float y, float z, float w)
	{
		(*quat)[0] = x;
		(*quat)[1] = y;
		(*quat)[2] = z;
		(*quat)[3] = w;

		return quat;
	}

	inline QUAT* QuatCopy(QUAT *quatOut, const QUAT *quatIn)
	{
		(*quatOut)[0] = (*quatIn)[0];
		(*quatOut)[1] = (*quatIn)[1];
		(*quatOut)[2] = (*quatIn)[2];
		(*quatOut)[3] = (*quatIn)[3];

		return quatOut;
	}

	inline float QuatLength(const QUAT *quat)
	{
		return FastSqrt((*quat)[0] * (*quat)[0] + (*quat)[1] * (*quat)[1] + (*quat)[2] * (*quat)[2] + (*quat)[3] * (*quat)[3]);
	}

	inline float QuatLength2(const QUAT *quat)
	{
		return (*quat)[0] * (*quat)[0] + (*quat)[1] * (*quat)[1] + (*quat)[2] * (*quat)[2] + (*quat)[3] * (*quat)[3];
	}

	inline float QuatNormalize(QUAT *quat)
	{
		float len;
		float invLen;

		len = QuatLength(quat);

		if (len > EPSILON_E3) {
			invLen = 1.0f / len;

			(*quat)[0] *= invLen;
			(*quat)[1] *= invLen;
			(*quat)[2] *= invLen;
			(*quat)[3] *= invLen;
		}

		return len;
	}

	inline QUAT* QuatConjugate(QUAT *quatOut, const QUAT *quatIn)
	{
		// 注意: 对于单位四元数来说四元数的倒数等于其共轭
		(*quatOut)[0] = -(*quatIn)[0];
		(*quatOut)[1] = -(*quatIn)[1];
		(*quatOut)[2] = -(*quatIn)[2];
		(*quatOut)[3] =  (*quatIn)[3];

		return quatOut;
	}

	inline QUAT* QuatAdd(QUAT *quatOut, const QUAT *quatIn1, const QUAT *quatIn2)
	{
		(*quatOut)[0] = (*quatIn1)[0] + (*quatIn2)[0];
		(*quatOut)[1] = (*quatIn1)[1] + (*quatIn2)[1];
		(*quatOut)[2] = (*quatIn1)[2] + (*quatIn2)[2];
		(*quatOut)[3] = (*quatIn1)[3] + (*quatIn2)[3];

		return quatOut;
	}

	inline QUAT* QuatSub(QUAT *quatOut, const QUAT *quatIn1, const QUAT *quatIn2)
	{
		(*quatOut)[0] = (*quatIn1)[0] - (*quatIn2)[0];
		(*quatOut)[1] = (*quatIn1)[1] - (*quatIn2)[1];
		(*quatOut)[2] = (*quatIn1)[2] - (*quatIn2)[2];
		(*quatOut)[3] = (*quatIn1)[3] - (*quatIn2)[3];

		return quatOut;
	}

	inline QUAT* QuatScale(QUAT *quatOut, const QUAT *quatIn, float scale)
	{
		(*quatOut)[0] = (*quatIn)[0] * scale;
		(*quatOut)[1] = (*quatIn)[1] * scale;
		(*quatOut)[2] = (*quatIn)[2] * scale;
		(*quatOut)[3] = (*quatIn)[3] * scale;

		return quatOut;
	}

	inline float QuatDot(const QUAT *quatIn1, const QUAT *quatIn2)
	{
		return (*quatIn1)[0] * (*quatIn2)[0] + (*quatIn1)[1] * (*quatIn2)[1] + (*quatIn1)[2] * (*quatIn2)[2] + (*quatIn1)[3] * (*quatIn2)[3];
	}

	inline QUAT* QuatMul(QUAT *quatOut, const QUAT *quatIn1, const QUAT *quatIn2)
	{
		// 四元数相乘
		// quat1 x quat2 = (quat1.xi + quat1.yj + quat1.zk + quat1.w)*(quat2.xi + quat2.yj + quat2.zk + quat1.w)
		//               = (quat1.w*quat2.x + quat1.x*quat2.w + quat1.y*quat2.z - quat1.z*quat2.y)i +
		//                 (quat1.w*quat2.y + quat1.y*quat2.w + quat1.z*quat2.x - quat1.x*quat2.z)j +
		//                 (quat1.w*quat2.z + quat1.z*quat2.w + quat1.x*quat2.y - quat1.y*quat2.x)k +
		//                 (quat1.w*quat2.w - quat1.x*quat2.x - quat1.y*quat2.y - quat1.z*quat2.z)
		//

		// 右乘及联
		//(*quatOut)[0] = (*quatIn1)[3]*(*quatIn2)[0] + (*quatIn1)[0]*(*quatIn2)[3] + (*quatIn1)[1]*(*quatIn2)[2] - (*quatIn1)[2]*(*quatIn2)[1];
		//(*quatOut)[1] = (*quatIn1)[3]*(*quatIn2)[1] + (*quatIn1)[1]*(*quatIn2)[3] + (*quatIn1)[2]*(*quatIn2)[0] - (*quatIn1)[0]*(*quatIn2)[2];
		//(*quatOut)[2] = (*quatIn1)[3]*(*quatIn2)[2] + (*quatIn1)[2]*(*quatIn2)[3] + (*quatIn1)[0]*(*quatIn2)[1] - (*quatIn1)[1]*(*quatIn2)[0];
		//(*quatOut)[3] = (*quatIn1)[3]*(*quatIn2)[3] - (*quatIn1)[0]*(*quatIn2)[0] - (*quatIn1)[1]*(*quatIn2)[1] - (*quatIn1)[2]*(*quatIn2)[2];

		// 左程及联
		(*quatOut)[0] = (*quatIn2)[3] * (*quatIn1)[0] + (*quatIn2)[0] * (*quatIn1)[3] + (*quatIn2)[1] * (*quatIn1)[2] - (*quatIn2)[2] * (*quatIn1)[1];
		(*quatOut)[1] = (*quatIn2)[3] * (*quatIn1)[1] + (*quatIn2)[1] * (*quatIn1)[3] + (*quatIn2)[2] * (*quatIn1)[0] - (*quatIn2)[0] * (*quatIn1)[2];
		(*quatOut)[2] = (*quatIn2)[3] * (*quatIn1)[2] + (*quatIn2)[2] * (*quatIn1)[3] + (*quatIn2)[0] * (*quatIn1)[1] - (*quatIn2)[1] * (*quatIn1)[0];
		(*quatOut)[3] = (*quatIn2)[3] * (*quatIn1)[3] - (*quatIn2)[0] * (*quatIn1)[0] - (*quatIn2)[1] * (*quatIn1)[1] - (*quatIn2)[2] * (*quatIn1)[2];

		return quatOut;
	}

	inline VEC3* Vec3MulQuat(VEC3 *vecOut, const VEC3 *vecIn, const QUAT *quatIn)
	{
		VEC3 qvec;
		VEC3 uv, uuv;

		Vec3Set(&qvec, (*quatIn)[0], (*quatIn)[1], (*quatIn)[2]);
		Vec3Cross(&uv, &qvec, vecIn);
		Vec3Cross(&uuv, &qvec, &uv);

		(*vecOut)[0] = (*vecIn)[0] + 2.0f*(uv[0] * (*quatIn)[3] + uuv[0]);
		(*vecOut)[1] = (*vecIn)[1] + 2.0f*(uv[1] * (*quatIn)[3] + uuv[1]);
		(*vecOut)[2] = (*vecIn)[2] + 2.0f*(uv[2] * (*quatIn)[3] + uuv[2]);

		return vecOut;
	}

	inline QUAT* QuatInverse(QUAT *quatOut, const QUAT *quatIn)
	{
		float len2;
		float invLen;

		len2 = (*quatIn)[0] * (*quatIn)[0] + (*quatIn)[1] * (*quatIn)[1] + (*quatIn)[2] * (*quatIn)[2] + (*quatIn)[3] * (*quatIn)[3];

		if (len2 > EPSILON_E3) {
			invLen = 1.0f / len2;

			(*quatOut)[0] = -(*quatIn)[0] * invLen;
			(*quatOut)[1] = -(*quatIn)[1] * invLen;
			(*quatOut)[2] = -(*quatIn)[2] * invLen;
			(*quatOut)[3] =  (*quatIn)[3] * invLen;
		}
		else {
			(*quatOut)[0] = -(*quatIn)[0];
			(*quatOut)[1] = -(*quatIn)[1];
			(*quatOut)[2] = -(*quatIn)[2];
			(*quatOut)[3] =  (*quatIn)[3];
		}

		return quatOut;
	}

	inline QUAT* QuatDiv(QUAT *quatOut, const QUAT *quatIn1, const QUAT *quatIn2)
	{
		QUAT inv;

		QuatInverse(&inv, quatIn2);
		QuatMul(quatOut, &inv, quatIn1);

		return quatOut;
	}

	inline QUAT* QuatLogN(QUAT *quatOut, const QUAT *quatIn)
	{
		float theta, scale;

#ifdef _DEBUG

		scale = (*quatIn)[0] * (*quatIn)[0] + (*quatIn)[1] * (*quatIn)[1] + (*quatIn)[2] * (*quatIn)[2] + (*quatIn)[3] * (*quatIn)[3];
		ASSERT(fabs(scale) - 1.0f < EPSILON_E3);

#endif

		scale = FastSqrt((*quatIn)[0] * (*quatIn)[0] + (*quatIn)[1] * (*quatIn)[1] + (*quatIn)[2] * (*quatIn)[2]);

		if (scale > EPSILON_E3) {
			theta = FastATan2(scale, (*quatIn)[3]);
			scale = theta / scale;
		}

		(*quatOut)[0] = (*quatIn)[0] * scale;
		(*quatOut)[1] = (*quatIn)[1] * scale;
		(*quatOut)[2] = (*quatIn)[2] * scale;
		(*quatOut)[3] = 0.0f;

		return quatOut;
	}

	inline QUAT* QuatExp(QUAT *quatOut, const QUAT *quatIn)
	{
		float theta, scale, sine;

		theta = FastSqrt((*quatIn)[0] * (*quatIn)[0] + (*quatIn)[1] * (*quatIn)[1] + (*quatIn)[2] * (*quatIn)[2]);
		scale = 1.0f;

		if (theta > EPSILON_E3) {
			sine = FastSin(theta);
			scale = sine / theta;
		}

		(*quatOut)[0] = (*quatIn)[0] * scale;
		(*quatOut)[1] = (*quatIn)[1] * scale;
		(*quatOut)[2] = (*quatIn)[2] * scale;
		(*quatOut)[3] = FastCos(theta);

		return quatOut;
	}

	inline QUAT* QuatClosest(QUAT *quatOut, const QUAT *quatIn, const QUAT *quatCloseTo)
	{
		float dot;

		dot = QuatDot(quatIn, quatCloseTo);

		if (dot < 0.0f) {
			(*quatOut)[0] = -(*quatIn)[0];
			(*quatOut)[1] = -(*quatIn)[1];
			(*quatOut)[2] = -(*quatIn)[2];
			(*quatOut)[3] = -(*quatIn)[3];
		}
		else {
			(*quatOut)[0] = (*quatIn)[0];
			(*quatOut)[1] = (*quatIn)[1];
			(*quatOut)[2] = (*quatIn)[2];
			(*quatOut)[3] = (*quatIn)[3];
		}

		return quatOut;
	}

	inline QUAT* QuatDefRotateAxisAngle(QUAT *quatOut, const VEC3 *vecInAxis, float rad)
	{
		float sine, cosine;
		VEC3 axis;

		sine = FastSin(rad*0.5f);
		cosine = FastCos(rad*0.5f);

		Vec3Copy(&axis, vecInAxis);
		Vec3Normalize(&axis);

		(*quatOut)[0] = axis[0] * sine;
		(*quatOut)[1] = axis[1] * sine;
		(*quatOut)[2] = axis[2] * sine;
		(*quatOut)[3] = cosine;

		QuatNormalize(quatOut);

		return quatOut;
	}

	inline QUAT* QuatDefRotateAngleXYZ(QUAT *quatOut, float radX, float radY, float radZ)
	{
		QUAT quatX, quatY, quatZ, quatXZ;

		QuatDefRotateAxisAngle(&quatX, &axisx, radX);
		QuatDefRotateAxisAngle(&quatY, &axisy, radY);
		QuatDefRotateAxisAngle(&quatZ, &axisz, radZ);

		QuatMul(&quatXZ, &quatX, &quatZ);
		QuatMul(quatOut, &quatY, &quatXZ);

		return quatOut;
	}

	inline QUAT* QuatDefLookAt(QUAT *quatOut, const VEC3 *directionIn, const VEC3 *upIn)
	{
		VEC3 target;
		VEC3 position;
		MATRIX4 mtxRotate;

		Vec3Set(&position, 0.0f, 0.0f, 0.0f);
		Vec3Ma(&target, &position, directionIn, 1.0f);

		MtxDefLookAt(&mtxRotate, &position, upIn, &target);
		MtxToQuat(quatOut, &mtxRotate);

		return quatOut;
	}

	inline QUAT* QuatLerp(QUAT *quatOut, const QUAT *quatIn1, const QUAT *quatIn2, float t)
	{
		(*quatOut)[0] = (*quatIn1)[0] + t*((*quatIn2)[0] - (*quatIn1)[0]);
		(*quatOut)[1] = (*quatIn1)[1] + t*((*quatIn2)[1] - (*quatIn1)[1]);
		(*quatOut)[2] = (*quatIn1)[2] + t*((*quatIn2)[2] - (*quatIn1)[2]);
		(*quatOut)[3] = (*quatIn1)[3] + t*((*quatIn2)[3] - (*quatIn1)[3]);

		return quatOut;
	}

	inline QUAT* QuatSlerp(QUAT *quatOut, const QUAT *quatIn1, const QUAT *quatIn2, float t)
	{
		float rad;
		float tp, tq;
		float sine, invSine, cosine;

		tp = 1.0f;
		tq = 1.0f;
		cosine = QuatDot(quatIn1, quatIn2);

		if (cosine < 0.0f) {
			cosine = -cosine;
			tq = -tq;
		}

		if (cosine < 1.0f - EPSILON_E3) {
			rad = FastACos(cosine);

			sine = FastSin(rad);
			invSine = 1.0f / sine;

			tq *= FastSin(t*rad)*invSine;
			tp *= FastSin((1.0f - t)*rad)*invSine;
		}
		else {
			tp = 1.0f - t;
			tq = tq*t;
		}

		(*quatOut)[0] = tp*(*quatIn1)[0] + tq*(*quatIn2)[0];
		(*quatOut)[1] = tp*(*quatIn1)[1] + tq*(*quatIn2)[1];
		(*quatOut)[2] = tp*(*quatIn1)[2] + tq*(*quatIn2)[2];
		(*quatOut)[3] = tp*(*quatIn1)[3] + tq*(*quatIn2)[3];

		return quatOut;
	}
	/*
	inline PxQuat PxQuatSlerp(const PxQuat &quatIn1, const PxQuat &quatIn2, float t)
	{
		PxQuat out;

		float rad;
		float tp, tq;
		float sine, invSine, cosine;

		tp = 1.0f;
		tq = 1.0f;
		cosine = quatIn1.x*quatIn2.x + quatIn1.y*quatIn2.y + quatIn1.z*quatIn2.z + quatIn1.w*quatIn2.w;

		if (cosine < 0.0f) {
			cosine = -cosine;
			tq = -tq;
		}

		if (cosine < 1.0f - EPSILON_E3) {
			rad = FastACos(cosine);

			sine = FastSin(rad);
			invSine = 1.0f / sine;

			tq *= FastSin(t*rad)*invSine;
			tp *= FastSin((1.0f - t)*rad)*invSine;
		}
		else {
			tp = 1.0f - t;
			tq = tq*t;
		}

		out.x = tp*quatIn1.x + tq*quatIn2.x;
		out.y = tp*quatIn1.y + tq*quatIn2.y;
		out.z = tp*quatIn1.z + tq*quatIn2.z;
		out.w = tp*quatIn1.w + tq*quatIn2.w;

		return out;
	}
	*/
	inline QUAT* QuatSquad(QUAT *quatOut, const QUAT *quatInFirst, const QUAT *quatInA, const QUAT *quatInB, const QUAT *quatInEnd, float t)
	{
		float t2;
		QUAT quatFirstEnd, quatAB;

		t2 = 2.0f*t*(1.0f - t);

		QuatSlerp(&quatFirstEnd, quatInFirst, quatInEnd, t);
		QuatSlerp(&quatAB, quatInA, quatInB, t);
		QuatSlerp(quatOut, &quatFirstEnd, &quatAB, t2);

		return quatOut;
	}

	inline QUAT* QuatComputeA(QUAT *quatOut, const QUAT *quatInPrev, const QUAT *quatInCurr, const QUAT *quatInNext)
	{
		QUAT qm, qp, lqm, lqp, qpqm, exq;

		QuatDiv(&qm, quatInPrev, quatInCurr);
		QuatLogN(&lqm, &qm);
		QuatDiv(&qp, quatInNext, quatInCurr);
		QuatLogN(&lqp, &qp);

		QuatAdd(&qpqm, &lqp, &lqm);
		QuatScale(&qpqm, &qpqm, -0.25f);

		QuatExp(&exq, &qpqm);
		QuatMul(quatOut, quatInCurr, &exq);

		return quatOut;
	}

	inline MATRIX4* QuatToMtxRotation(MATRIX4 *mtxOut, const QUAT *quatIn)
	{
		//  _                                              _
		// | 1-2(y2 + z2)     2(xy + wz)     2(xz - wy)   0 |
		// |   2(xy - wz)   1-2(x2 + z2)     2(yz + wx)   0 |
		// |   2(xz + wy)     2(yz - wx)   1-2(x2 + y2)   0 |
		// |_       0              0              0       1_|
		//

		float x2, y2, z2;
		float xy, xz, yz, wy, wx, wz;

		x2 = (*quatIn)[0] * (*quatIn)[0] * 2.0f;
		y2 = (*quatIn)[1] * (*quatIn)[1] * 2.0f;
		z2 = (*quatIn)[2] * (*quatIn)[2] * 2.0f;
		xy = (*quatIn)[0] * (*quatIn)[1] * 2.0f;
		xz = (*quatIn)[0] * (*quatIn)[2] * 2.0f;
		yz = (*quatIn)[1] * (*quatIn)[2] * 2.0f;
		wx = (*quatIn)[3] * (*quatIn)[0] * 2.0f;
		wy = (*quatIn)[3] * (*quatIn)[1] * 2.0f;
		wz = (*quatIn)[3] * (*quatIn)[2] * 2.0f;

		MtxSet(mtxOut,
			1.0f - (y2 + z2), xy + wz,          xz - wy,          0.0f,
			xy - wz,          1.0f - (x2 + z2), yz + wx,          0.0f,
			xz + wy,          yz - wx,          1.0f - (x2 + y2), 0.0f,
			0.0f,             0.0f,             0.0f,             1.0f);

		return mtxOut;
	}

	inline void QuatToAngleXYZ(float *radXOut, float *radYOut, float *radZOut, const QUAT *quatIn)
	{
		float a, b, s;

		s = ((*quatIn)[0] * (*quatIn)[3] - (*quatIn)[1] * (*quatIn)[2])*2.0f;

		// 万向锁检查, 向上或向下看
		if (s >= 1.0f - EPSILON_E3 || s <= -1.0f + EPSILON_E3) {
			if (radXOut) *radXOut = s > 0.0f ? HALF_PI : -HALF_PI;

			a = ((*quatIn)[1] * (*quatIn)[3] - (*quatIn)[0] * (*quatIn)[2])*2.0f;
			b = 1.0f - ((*quatIn)[1] * (*quatIn)[1] + (*quatIn)[2] * (*quatIn)[2])*2.0f;
			if (radYOut) *radYOut = FastATan2(a, b);

			if (radZOut) *radZOut = 0.0f;
		}
		else {
			if (radXOut) *radXOut = FastASin(s);

			a = ((*quatIn)[1] * (*quatIn)[3] + (*quatIn)[0] * (*quatIn)[2])*2.0f;
			b = 1.0f - ((*quatIn)[0] * (*quatIn)[0] + (*quatIn)[1] * (*quatIn)[1])*2.0f;
			if (radYOut) *radYOut = FastATan2(a, b);

			a = ((*quatIn)[2] * (*quatIn)[3] + (*quatIn)[0] * (*quatIn)[1])*2.0f;
			b = 1.0f - ((*quatIn)[0] * (*quatIn)[0] + (*quatIn)[2] * (*quatIn)[2])*2.0f;
			if (radZOut) *radZOut = FastATan2(a, b);
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	// 平面运算
	//=============================================================================
	inline void PlaneZero(PLANE *plane)
	{
		Vec3Zero(&plane->vertex);
		Vec3Zero(&plane->normal);
	}

	inline PLANE* PlaneCopy(PLANE *planeOut, const PLANE *planeIn)
	{
		Vec3Copy(&planeOut->vertex, &planeIn->vertex);
		Vec3Copy(&planeOut->normal, &planeIn->normal);

		return planeOut;
	}

	inline PLANE* PlaneSet(PLANE *planeOut, const VEC3 *vertexIn, const VEC3 *normalIn)
	{
		Vec3Copy(&planeOut->vertex, vertexIn);
		Vec3Copy(&planeOut->normal, normalIn);
		Vec3Normalize(&planeOut->normal);

		return planeOut;
	}

	inline float VertexToPlaneDistance(const PLANE *planeIn, const VEC3 *vertexIn)
	{
		VEC3 delta;

		Vec3Sub(&delta, vertexIn, &planeIn->vertex);
		return Vec3Dot(&planeIn->normal, &delta);
	}

	inline VEC3* VertexProjectionToPlane(VEC3 *projectionOut, const PLANE *planeIn, const VEC3 *vertexIn)
	{
		float a;
		float b;
		VEC3 tmp;

		a = Vec3Dot(vertexIn, &planeIn->normal);
		b = -Vec3Dot(&planeIn->vertex, &planeIn->normal);

		Vec3Scale(&tmp, &planeIn->normal, a + b);
		Vec3Sub(projectionOut, vertexIn, &tmp);

		return projectionOut;
	}

	inline BOOL LineIntersectPlane(const VEC3 *vertex0, const VEC3 *vertex1, const PLANE *plane, VEC3 *cross, float *t)
	{
		float a, b, tt;

		b = plane->normal[0] * ((*vertex1)[0] - (*vertex0)[0]) +
			plane->normal[1] * ((*vertex1)[1] - (*vertex0)[1]) +
			plane->normal[2] * ((*vertex1)[2] - (*vertex0)[2]);

		if (fabs(b) < EPSILON_E3) {
			return FALSE;
		}

		a = plane->normal[0] * (plane->vertex[0] - (*vertex0)[0]) +
			plane->normal[1] * (plane->vertex[1] - (*vertex0)[1]) +
			plane->normal[2] * (plane->vertex[2] - (*vertex0)[2]);

		tt = a / b;

		if (tt < 0.0f || tt > 1.0f) {
			return FALSE;
		}

		if (t) {
			*t = tt;
		}

		if (cross) {
			(*cross)[0] = (*vertex0)[0] + ((*vertex1)[0] - (*vertex0)[0])*tt;
			(*cross)[1] = (*vertex0)[1] + ((*vertex1)[1] - (*vertex0)[1])*tt;
			(*cross)[2] = (*vertex0)[2] + ((*vertex1)[2] - (*vertex0)[2])*tt;
		}

		return TRUE;
	}

	inline PLANE_SIDE VertexPlaneSide(const VEC3 *vertex, const PLANE *plane)
	{
		float d;
		VEC3 vec;

		Vec3Sub(&vec, vertex, &plane->vertex);
		d = Vec3Dot(&vec, &plane->normal);

		if (d < -EPSILON_E3) return PLANE_SIDE_BACK;
		if (d > +EPSILON_E3) return PLANE_SIDE_FRONT;

		return PLANE_SIDE_IN;
	}


	///////////////////////////////////////////////////////////////////////////////
	// 碰撞检测
	//=============================================================================
	inline SPHERE* CreateSphere(SPHERE *sphere, const VEC3 *vertices, int numVertices)
	{
		// 算法: 
		//    1. 找到x, y, z轴线上最远的顶点和最近的顶点(注意: 与AABB的
		//       MAXx,MAXy,MAXz,MINx,MINy,MINz不同)
		//    2. 从这三组顶点中找出距离最大的一组顶点, 取这组顶点的中点作
		//       为球体的球心, 取他们距离的一半作为半径r
		//    3. 便利所有顶点, 计算他们到球心的距离d, 如果d > r, 则将球心
		//       朝这个方向移动(d-r)/2, 同时半径大小改为(d+r)/2
		//
		// 参考:
		//    <<实时计算机图形学(第二版)>> P315 13.3包围体的创建

		//
		// 1. 找到x, y, z轴线上最远的顶点和最近的顶点
		//
		int indexMax[3];
		int indexMin[3];

		float max[3], min[3];

		min[0] = min[1] = min[2] = FLT_MAX;
		max[0] = max[1] = max[2] = -FLT_MAX;

		for (int index = 0; index < numVertices; index++) {
			if (min[0] > vertices[index][0]) { min[0] = vertices[index][0]; indexMin[0] = index; }
			if (min[1] > vertices[index][1]) { min[1] = vertices[index][1]; indexMin[1] = index; }
			if (min[2] > vertices[index][2]) { min[2] = vertices[index][2]; indexMin[2] = index; }

			if (max[0] < vertices[index][0]) { max[0] = vertices[index][0]; indexMax[0] = index; }
			if (max[1] < vertices[index][1]) { max[1] = vertices[index][1]; indexMax[1] = index; }
			if (max[2] < vertices[index][2]) { max[2] = vertices[index][2]; indexMax[2] = index; }
		}

		//
		// 2. 从这三组定点中找出距离最大的一组顶点, 取这组顶点的中点作
		//    为球体的球心, 取他们距离的一半作为半径r
		//
		sphere->radius2 = -FLT_MAX;

		for (int index = 0; index < 3; index++) {
			float d2;
			VEC3 vec;

			Vec3Sub(&vec, &vertices[indexMax[index]], &vertices[indexMin[index]]);
			d2 = Vec3Length2(&vec);

			if (sphere->radius2 < d2) {
				sphere->radius2 = d2;

				sphere->center[0] = (vertices[indexMax[index]][0] + vertices[indexMin[index]][0])*0.5f;
				sphere->center[1] = (vertices[indexMax[index]][1] + vertices[indexMin[index]][1])*0.5f;
				sphere->center[2] = (vertices[indexMax[index]][2] + vertices[indexMin[index]][2])*0.5f;
			}
		}

		sphere->radius2 *= 0.25f;

		//
		// 3. 遍历所有顶点, 计算他们到球心的距离d, 如果d > r, 则将球心
		//    朝这个方向移动(d-r)/2, 同时半径大小改为(d+r)/2
		//
		for (int index = 0; index < numVertices; index++) {
			float d2;
			VEC3 vec;

			Vec3Sub(&vec, &vertices[index], &sphere->center);
			d2 = Vec3Length2(&vec);

			if (sphere->radius2 < d2) {
				//
				// 计算球心
				// offset = (d - r)/2;
				//        = sqrt(((d - r)/2)^2)
				//        = sqrt((d2 - 2rd + r2)/4)
				//
				float rd = FastSqrt(sphere->radius2*d2);
				float offset = FastSqrt((d2 - 2.0f*rd + sphere->radius2)*0.25f);

				Vec3Normalize(&vec);
				Vec3Ma(&sphere->center, &sphere->center, &vec, offset);

				//
				// 计算半径
				// r   = (d + r)/2
				// r^2 = (d2 + 2rd + r2)/4
				//
				sphere->radius2 = (d2 + 2.0f*rd + sphere->radius2)*0.25f;
			}
		}

		sphere->radius = FastSqrt(sphere->radius2);

		return sphere;
	}

	inline AABB* CreateAABB(AABB *aabb, float minx, float miny, float minz, float maxx, float maxy, float maxz)
	{
		aabb->minVertex[0] = minx < maxx ? minx : maxx;
		aabb->minVertex[1] = miny < maxy ? miny : maxy;
		aabb->minVertex[2] = minz < maxz ? minz : maxz;

		aabb->maxVertex[0] = minx > maxx ? minx : maxx;
		aabb->maxVertex[1] = miny > maxy ? miny : maxy;
		aabb->maxVertex[2] = minz > maxz ? minz : maxz;

		aabb->center[0] = (aabb->minVertex[0] + aabb->maxVertex[0])*0.5f;
		aabb->center[1] = (aabb->minVertex[1] + aabb->maxVertex[1])*0.5f;
		aabb->center[2] = (aabb->minVertex[2] + aabb->maxVertex[2])*0.5f;

		return aabb;
	}

	inline AABB* CreateAABB(AABB *aabb, const VEC3 *min, const VEC3 *max)
	{
		return CreateAABB(aabb, (*min)[0], (*min)[1], (*min)[2], (*max)[0], (*max)[1], (*max)[2]);
	}

	inline AABB* CreateAABB(AABB *aabb, const VEC3 *vertices, int numVertices)
	{
		// 算法: 计算x, y, z轴向上最大值和最小值, 以此作为包围盒区域
		//
		// 参考:
		//    <<实时计算机图形学(第二版)>> P315 13.3包围体的创建

		aabb->minVertex[0] = aabb->minVertex[1] = aabb->minVertex[2] = FLT_MAX;
		aabb->maxVertex[0] = aabb->maxVertex[1] = aabb->maxVertex[2] = -FLT_MAX;

		for (int index = 0; index < numVertices; index++) {
			if (aabb->minVertex[0] > vertices[index][0]) aabb->minVertex[0] = vertices[index][0];
			if (aabb->minVertex[1] > vertices[index][1]) aabb->minVertex[1] = vertices[index][1];
			if (aabb->minVertex[2] > vertices[index][2]) aabb->minVertex[2] = vertices[index][2];

			if (aabb->maxVertex[0] < vertices[index][0]) aabb->maxVertex[0] = vertices[index][0];
			if (aabb->maxVertex[1] < vertices[index][1]) aabb->maxVertex[1] = vertices[index][1];
			if (aabb->maxVertex[2] < vertices[index][2]) aabb->maxVertex[2] = vertices[index][2];
		}

		aabb->center[0] = (aabb->minVertex[0] + aabb->maxVertex[0])*0.5f;
		aabb->center[1] = (aabb->minVertex[1] + aabb->maxVertex[1])*0.5f;
		aabb->center[2] = (aabb->minVertex[2] + aabb->maxVertex[2])*0.5f;

		return aabb;
	}

	inline BOOL IsAABBValid(const AABB *aabb)
	{
		float width = aabb->maxVertex[0] - aabb->minVertex[0];
		float height = aabb->maxVertex[1] - aabb->minVertex[1];
		float length = aabb->maxVertex[2] - aabb->minVertex[2];
		return width > EPSILON_E3 || height > EPSILON_E3 || length > EPSILON_E3 ? TRUE : FALSE;
	}

	inline BOOL IsPointInAABB(const AABB *aabb, float x, float y, float z)
	{
		return
			x >= aabb->minVertex[0] && x <= aabb->maxVertex[0] &&
			y >= aabb->minVertex[1] && y <= aabb->maxVertex[1] &&
			z >= aabb->minVertex[2] && z <= aabb->maxVertex[2] ? TRUE : FALSE;
	}

	inline void AABBZero(AABB *aabb)
	{
		Vec3Zero(&aabb->center);
		Vec3Zero(&aabb->minVertex);
		Vec3Zero(&aabb->maxVertex);
	}

	inline void AABBCopy(AABB *aabbOut, const AABB *aabbIn)
	{
		Vec3Copy(&aabbOut->center, &aabbIn->center);
		Vec3Copy(&aabbOut->minVertex, &aabbIn->minVertex);
		Vec3Copy(&aabbOut->maxVertex, &aabbIn->maxVertex);
	}

	inline AABB* AABBTransform(AABB *aabbOut, const AABB *aabbIn, const MATRIX4 *mtxTranform)
	{
		VEC3 vertices[8];

		Vec3Set(&vertices[0], aabbIn->minVertex[0], aabbIn->minVertex[1], aabbIn->minVertex[2]);
		Vec3Set(&vertices[1], aabbIn->maxVertex[0], aabbIn->minVertex[1], aabbIn->minVertex[2]);
		Vec3Set(&vertices[2], aabbIn->maxVertex[0], aabbIn->minVertex[1], aabbIn->maxVertex[2]);
		Vec3Set(&vertices[3], aabbIn->minVertex[0], aabbIn->minVertex[1], aabbIn->maxVertex[2]);
		Vec3Set(&vertices[4], aabbIn->minVertex[0], aabbIn->maxVertex[1], aabbIn->minVertex[2]);
		Vec3Set(&vertices[5], aabbIn->maxVertex[0], aabbIn->maxVertex[1], aabbIn->minVertex[2]);
		Vec3Set(&vertices[6], aabbIn->maxVertex[0], aabbIn->maxVertex[1], aabbIn->maxVertex[2]);
		Vec3Set(&vertices[7], aabbIn->minVertex[0], aabbIn->maxVertex[1], aabbIn->maxVertex[2]);

		aabbOut->minVertex[0] = aabbOut->minVertex[1] = aabbOut->minVertex[2] = FLT_MAX;
		aabbOut->maxVertex[0] = aabbOut->maxVertex[1] = aabbOut->maxVertex[2] = -FLT_MAX;

		for (int indexVertex = 0; indexVertex < 8; indexVertex++) {
			VEC3 vertex;
			Vec3MulMtx4x4(&vertex, &vertices[indexVertex], mtxTranform);

			if (aabbOut->minVertex[0] > vertex[0]) aabbOut->minVertex[0] = vertex[0];
			if (aabbOut->minVertex[1] > vertex[1]) aabbOut->minVertex[1] = vertex[1];
			if (aabbOut->minVertex[2] > vertex[2]) aabbOut->minVertex[2] = vertex[2];

			if (aabbOut->maxVertex[0] < vertex[0]) aabbOut->maxVertex[0] = vertex[0];
			if (aabbOut->maxVertex[1] < vertex[1]) aabbOut->maxVertex[1] = vertex[1];
			if (aabbOut->maxVertex[2] < vertex[2]) aabbOut->maxVertex[2] = vertex[2];
		}

		aabbOut->center[0] = (aabbOut->minVertex[0] + aabbOut->maxVertex[0])*0.5f;
		aabbOut->center[1] = (aabbOut->minVertex[1] + aabbOut->maxVertex[1])*0.5f;
		aabbOut->center[2] = (aabbOut->minVertex[2] + aabbOut->maxVertex[2])*0.5f;

		return aabbOut;
	}

	inline AABB* AABBNormalize(AABB *aabb)
	{
		VEC3 minVertex;
		VEC3 maxVertex;

		minVertex[0] = min(aabb->minVertex[0], aabb->maxVertex[0]);
		minVertex[1] = min(aabb->minVertex[1], aabb->maxVertex[1]);
		minVertex[2] = min(aabb->minVertex[2], aabb->maxVertex[2]);

		maxVertex[0] = max(aabb->minVertex[0], aabb->maxVertex[0]);
		maxVertex[1] = max(aabb->minVertex[1], aabb->maxVertex[1]);
		maxVertex[2] = max(aabb->minVertex[2], aabb->maxVertex[2]);

		aabb->minVertex[0] = minVertex[0];
		aabb->minVertex[1] = minVertex[1];
		aabb->minVertex[2] = minVertex[2];

		aabb->maxVertex[0] = maxVertex[0];
		aabb->maxVertex[1] = maxVertex[1];
		aabb->maxVertex[2] = maxVertex[2];

		aabb->center[0] = (minVertex[0] + maxVertex[0])*0.5f;
		aabb->center[1] = (minVertex[1] + maxVertex[1])*0.5f;
		aabb->center[2] = (minVertex[2] + maxVertex[2])*0.5f;

		return aabb;
	}

	inline BOOL SphereIntersect(const SPHERE *sphere0, const SPHERE *sphere1)
	{
		// 算法: 计算两球心距离的平方, 如果距离平方小于两球半径和平方
		//       则两球相交, 反之不相交

		//
		// 1. 计算球心距离平方
		//
		float d2;
		VEC3 vec;

		Vec3Sub(&vec, &sphere0->center, &sphere1->center);
		d2 = Vec3Length2(&vec);

		//
		// 2. 比较球心距离与两球半径距离和
		//    d^2 <= r0^2 + 2*r0* + r1^2r1
		//
		return d2 <= sphere0->radius2 + 2.0f*sphere0->radius*sphere1->radius + sphere1->radius2;
	}

	inline BOOL SphereIntersectAABB(const SPHERE *sphere, const AABB *aabb)
	{
		// 算法: 在AABB上找到距离球心c最近的一个点, 其中需要对AABB的每个轴线都进行
		//       一次一维测试, 判断球心坐标每个分量是否在AABB的边界之内, 如果在AABB
		//       的边界之外, 那么计算出球心沿着这条轴线与这个长方体的距离(这是一个
		//       减法操作), 然后计算出这个距离的平方, 在这3条轴线全部计算完之后, 
		//       将这个距离的平方加起来同球体半径比较, 如果小于球体半径则在球之内, 
		//       反之在球之外

		//
		// 1. 计算球心到包围盒最远顶点的距离
		//
		float d2 = 0.0f;

		for (int index = 0; index < 3; index++) {
			if (sphere->center[index] < aabb->minVertex[index]) {
				float delta = sphere->center[index] - aabb->minVertex[index];
				d2 = d2 + delta*delta;
			}
			else if (sphere->center[index] > aabb->maxVertex[index]) {
				float delta = sphere->center[index] - aabb->maxVertex[index];
				d2 = d2 + delta*delta;
			}
		}

		//
		// 2. 比较球心到包围盒最远顶点的距离与球半径平方和
		//
		return d2 <= sphere->radius2;
	}

	inline BOOL AABBIntersect(const AABB *aabb0, const AABB *aabb1)
	{
		// 算法: 在每个维度上检查包围和是否相交, 如果都相交则包围盒相交
		//       反之不相交

		for (int index = 0; index < 3; index++) {
			if (aabb0->minVertex[index] > aabb1->maxVertex[index] + EPSILON_E3 || 
				aabb1->minVertex[index] > aabb0->maxVertex[index] + EPSILON_E3) {
				return FALSE;
			}
		}

		return TRUE;
	}

	inline BOOL RayIntersectPlane(const VEC3 *origin, const VEC3 *dir, const PLANE *plane, float *length)
	{
		float a, b, len;

		b = plane->normal[0] * (*dir)[0] +
			plane->normal[1] * (*dir)[1] +
			plane->normal[2] * (*dir)[2];

		if (fabs(b) < EPSILON_E3) {
			return FALSE;
		}

		a = plane->normal[0] * (plane->vertex[0] - (*origin)[0]) +
			plane->normal[1] * (plane->vertex[1] - (*origin)[1]) +
			plane->normal[2] * (plane->vertex[2] - (*origin)[2]);

		len = a / b;

		if (len < 0.0f) {
			return FALSE;
		}

		if (length) {
			if (*length > len) {
				*length = len;
				return TRUE;
			}

			return FALSE;
		}

		return TRUE;
	}

	inline BOOL RayIntersectSphere(const VEC3 *origin, const VEC3 *dir, const SPHERE *sphere, float *length)
	{
		// 算法: 在满足以下两个条件可判断射线与球不相交
		//       1. 射线原点到球心向量与射线方向反向, 且射线原点到球心距离大于球
		//          半径, 则射线与球不相交
		//       2. 球心到射线距离大于球半径则射线与球不相交
		//
		// 说明: 仅根据第二个条件便可判断其相交性, 但第一条运算量较少且可以计算出
		//       第二条所需要的变量, 所以先进行第一条判定, 然后如有需要进一步进行
		//       第二条判定
		//
		// 参考:
		//    <<实时计算机图形学(第二版)>> P317 13.5射线/球体相交测试

		//
		// 1. 射线原点到球心向量与射线方向反向, 且射线原点到球心距离大于球
		//    半径, 则射线与球不相交
		//
		VEC3 vec;

		Vec3Sub(&vec, &sphere->center, origin);

		float s = Vec3Dot(&vec, dir);
		float d2 = Vec3Length2(&vec);

		if (s <= 0.0f && d2 > sphere->radius2) {
			return FALSE;
		}

		//
		// 2. 球心到射线距离大于球半径则射线与球不相交
		//
		float s2 = s*s;
		float m2 = d2 - s2; // 勾股定理

		if (m2 > sphere->radius2) {
			return FALSE;
		}

		//
		// 3. 计算射线到球的距离
		//
		if (length) {
			float q = FastSqrt(sphere->radius2 - m2); // 勾股定理
			float len = s + (d2 > sphere->radius2 ? -q : q);

			if (*length > len) {
				*length = len;
				return TRUE;
			}

			return FALSE;
		}

		return TRUE;
	}

	inline BOOL RayIntersectAABB(const VEC3 *origin, const VEC3 *dir, const AABB *aabb, float *length)
	{
		// 算法: 平行层
		//       将长方体看成3个平板层计算射线与平板层的交点对于每个平板层, 存
		//       在一个最小的t值和最大的t值, 分别记做timin和timax, i∈{u,v,w}, 计算
		//            tmin=max(tumin,tvmin,twmin) tmax=min(tumax,tvmax,twmax)
		//       然后就可以进行巧妙测试: 如果timin<=timax, 射线与长方体相交, 否则不相交
		//
		// 如图:  
		//  ______\ x+
		// |      /       y      y
		// |          R0  |      |
		//\|/ y+ R1     \ |      |
		//        \      \|tymin |
		//         \      *      |
		//    txmin \     |\ txmin
		// ----------*----+-*----+------------ x
		//            \   |  \   |
		//       txmax \  |   \ txmax
		// -------------*-+----*-+------------ x
		//               \|     \|
		//          tymin *      *tymax
		//                |\     |\
		//                | \    | \  
		//                |  \   |
		//                |   \  |
		//                |    \ |
		//                |     \|
		//                |      * tymax
		//                |      |\
		//
		// R0 max(txmin, tymin) < min(txmax, tymax) 相交
		// R1 max(txmin, tymin) > min(txmax, tymax) 不相交
		//
		// 参考:
		//    <<实时计算机图形学(第二版)>> P319 13.6射线/长方体相交测试

		VEC3 p, h;

		Vec3Sub(&p, &aabb->center, origin);
		Vec3Sub(&h, &aabb->maxVertex, &aabb->center);

		float tmin = -FLT_MAX;
		float tmax = FLT_MAX;

		for (int index = 0; index < 3; index++) {
			float e = p[index];
			float l = h[index];
			float f = (*dir)[index];

			if (f < -EPSILON_E3 || f > EPSILON_E3) {
				float invf = 1.0f / f;

				float tmp;
				float t1 = e + l;
				float t2 = e - l;

				t1 = t1*invf;
				t2 = t2*invf;
				if (t1 > t2) SWAP(t1, t2, tmp);

				if (tmin < t1) tmin = t1;
				if (tmax > t2) tmax = t2;

				if (tmin > tmax) {
					return FALSE;
				}
				if (tmax < 0.0f) {
					return FALSE;
				}
			}
			else if (-e - l > 0.0f || -e + l < 0.0f) {
				return FALSE;
			}
		}

		if (length) {
			float len = tmin > 0.0f ? tmin : tmax;

			if (*length > len) {
				*length = len;
				return TRUE;
			}

			return FALSE;
		}

		return TRUE;
	}

	inline BOOL RayIntersectTriangle(const VEC3 *origin, const VEC3 *dir, const VEC3 *v0, const VEC3 *v1, const VEC3 *v2, float *u, float *v, float *length)
	{
		float uu, vv;
		float dot, det, invDet;

		VEC3 edge1, edge2;
		VEC3 pvec, qvec, tvec;

		//
		// 1. Find vectors for two edges sharing vert0
		//
		Vec3Sub(&edge1, v1, v0);
		Vec3Sub(&edge2, v2, v0);

		//
		// 2. Begin calculating determinant - also used to calculate U parameter
		//
		Vec3Cross(&pvec, dir, &edge2);

		//
		// 3. If determinant is near zero, ray lies in plane of triangle
		//
		det = Vec3Dot(&edge1, &pvec);

		if (FastFAbs(det) < EPSILON_E3) {
			return FALSE;
		}

		if (det > 0.0f) {
			Vec3Sub(&tvec, origin, v0);
		}
		else {
			Vec3Sub(&tvec, v0, origin);
			det = -det;
		}

		//
		// 4. Calculate U parameter and test bounds
		//
		uu = Vec3Dot(&tvec, &pvec);

		if (uu < 0.0f || uu > det) {
			return FALSE;
		}

		//
		// 5. Prepare to test V parameter
		//
		Vec3Cross(&qvec, &tvec, &edge1);
		dot = Vec3Dot(&edge2, &qvec);

		if (dot < 0.0f) {
			return FALSE;
		}

		//
		// 6. Calculate V parameter and test bounds
		//
		vv = Vec3Dot(dir, &qvec);

		if (vv < 0.0f || uu + vv > det) {
			return FALSE;
		}

		//
		// 7. Calculate length, scale parameters, ray intersects triangle
		//
		invDet = 1.0f / det;

		if (u) {
			*u = uu*invDet;
		}

		if (v) {
			*v = vv*invDet;
		}

		if (length) {
			float len = dot*invDet;

			if (*length > len) {
				*length = len;

				if (u) *u = uu*invDet;
				if (v) *v = vv*invDet;

				return TRUE;
			}

			return FALSE;
		}
		else {
			if (u) *u = uu*invDet;
			if (v) *v = vv*invDet;
		}

		return TRUE;
	}

}

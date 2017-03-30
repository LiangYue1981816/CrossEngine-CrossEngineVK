/////////////////////////////////////////////////////////////////////
// resize.c
//
// 功  能: 缩放影像
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         09:01 2004-12-19
//         11:39 2005-06-19
//         12:01 2007-01-07
//         20:35 2007-01-07
//         16:43 2015-01-13
//===================================================================

#include "_image.h"

static void * resize1bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley);
static void * resize8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley);
static void * resize24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley);
static void * resize32bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Resize
//
// 功  能: 创建临时影像
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Resize(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	int x, y;
	int *tablex = NULL;
	int *tabley = NULL;
	float zoomx, zoomy;

	int itable;
	float ftable;

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (pimgsrc == pimgdst) { // 不能源对源
		return ERR_IMAGE;
	}
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}

	//
	// 1. 计算缩放查找表
	//
	tablex = (int *)calloc(IMAGE_WIDTH(pimgdst),  sizeof(int));
	tabley = (int *)calloc(IMAGE_HEIGHT(pimgdst), sizeof(int));
	if (NULL == tablex || NULL == tabley) { rcode = ERR_MEM; goto RET; }

	zoomx = (float)(1.0f*IMAGE_AREA_WIDTH(pimgsrc)/IMAGE_WIDTH(pimgdst));
	zoomy = (float)(1.0f*IMAGE_AREA_HEIGHT(pimgsrc)/IMAGE_HEIGHT(pimgdst));
	for (x = 0; x < IMAGE_WIDTH(pimgdst); x++) { ftable = x*zoomx+pimgsrc->sx; itable = _float2int(ftable); tablex[x] = itable; }
	for (y = 0; y < IMAGE_HEIGHT(pimgdst);y++) { ftable = y*zoomy+pimgsrc->sy; itable = _float2int(ftable); tabley[y] = itable; }

	//
	// 2. 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1: resize1bit(pimgsrc, pimgdst, tablex, tabley); break;
	case  8: resize8bit(pimgsrc, pimgdst, tablex, tabley); break;
	case 24: resize24bit(pimgsrc, pimgdst, tablex, tabley); break;
	case 32: resize32bit(pimgsrc, pimgdst, tablex, tabley); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	if (tablex) free(tablex);
	if (tabley) free(tabley);
	return rcode;
}

static void * resize1bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley)
{
	int x, y;
	int width, height;

	int xsrcbit, xdstbit;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		ppixel_dst = &pimgdst->matrix[y][0];
		ppixel_src = &pimgsrc->matrix[tabley[y]][0];

		xdstbit = 0;
		for (x = 0; x < width; x++) {
			xsrcbit = tablex[x]%8;

			if (0 == (ppixel_src[tablex[x]/8] & offsetbits1[xsrcbit]))
				*ppixel_dst &= offsetbits0[xdstbit]; // 设置为0
			else
				*ppixel_dst |= offsetbits1[xdstbit]; // 设置为1

			xdstbit++; if (8 == xdstbit) { ppixel_dst++; xdstbit = 0; }
		}
	}

	return NULL;
}

static void * resize8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0]));
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[tabley[y]][0]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[tablex[x]];
		}
	}

	return NULL;
}

static void * resize24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[y][0]));
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[tabley[y]][0]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[tablex[x]];
		}
	}

	return NULL;
}

static void * resize32bit(IMAGE *pimgsrc, IMAGE *pimgdst, int *tablex, int *tabley)
{
	int x, y;
	int width, height;

	IMGRGBA *ppixel_src = NULL;
	IMGRGBA *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGRGBA *)(&(pimgdst->matrix[y][0]));
		ppixel_src = (IMGRGBA *)(&(pimgsrc->matrix[tabley[y]][0]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[tablex[x]];
		}
	}

	return NULL;
}

static void * resizeex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *tablex, float *tabley);
static void * resizeex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *tablex, float *tabley);
static void * resizeex32bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *tablex, float *tabley);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_ResizeEx
//
// 功  能: 创建临时影像
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_ResizeEx(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	int x, y;
	float zoomx, zoomy;
	float *tablex = NULL;
	float *tabley = NULL;

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (pimgsrc == pimgdst) { // 不能源对源
		return ERR_IMAGE;
	}
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}

	//
	// 1. 计算缩放查找表
	//
	tablex = (float *)calloc(IMAGE_WIDTH(pimgdst),  sizeof(float));
	tabley = (float *)calloc(IMAGE_HEIGHT(pimgdst), sizeof(float));
	if (NULL == tablex || NULL == tabley) { rcode = ERR_MEM; goto RET; }

	zoomx = (float)(1.0f*IMAGE_AREA_WIDTH(pimgsrc)/IMAGE_WIDTH(pimgdst));
	zoomy = (float)(1.0f*IMAGE_AREA_HEIGHT(pimgsrc)/IMAGE_HEIGHT(pimgdst));
	for (x = 0; x < IMAGE_WIDTH(pimgdst); x++) tablex[x] = x*zoomx+pimgsrc->sx;
	for (y = 0; y < IMAGE_HEIGHT(pimgdst);y++) tabley[y] = y*zoomy+pimgsrc->sy;

	//
	// 2. 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: resizeex8bit(pimgsrc, pimgdst, tablex, tabley); break;
	case 24: resizeex24bit(pimgsrc, pimgdst, tablex, tabley); break;
	case 32: resizeex32bit(pimgsrc, pimgdst, tablex, tabley); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	if (tablex) free(tablex);
	if (tabley) free(tabley);
	return rcode;
}

static void * resizeex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *tablex, float *tabley)
{
	int x, y, x0, x1;
	int width, height;

	int ix, iy, ic;
	float fx, fy, fc;

	float u, v, vv, uu;

	IMGGRAY *ppixel_dst = NULL;
	IMGGRAY *ppixel_src0 = NULL; // this line
	IMGGRAY *ppixel_src1 = NULL; // bottom line

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		fy = tabley[y];
		iy = _float2int(fy);

		ppixel_dst  = (IMGGRAY *)(&(pimgdst->matrix[y][0]));
		ppixel_src0 = (IMGGRAY *)(&(pimgsrc->matrix[iy][0]));
		ppixel_src1 = fy + 1 <= pimgsrc->ey ? (IMGGRAY *)(&(pimgsrc->matrix[iy+1][0])) : ppixel_src0;
		
		// 双线内插值
		v = fy - iy; vv = 1 - v;
		for (x = 0; x < width; x++) {
			fx = tablex[x];
			ix = _float2int(fx);

			u = fx - ix; uu = 1 - u;
			x0 = ix; x1 = x0 + 1; if (x1 > (int)pimgsrc->ex) x1 = x0;

			// f(i,j)*(1-u)*(1-v) + f(i,j+1)*(1-u)*v + f(i+1,j)*u*(1-v) + f(i+1,j+1)*u*v
			fc = ppixel_src0[x0]*uu*vv + ppixel_src1[x0]*uu*v + ppixel_src0[x1]*vv*u + ppixel_src1[x1]*v*u; ic = _float2int(fc); ppixel_dst[x] = (IMGGRAY)ic;
		}
	}

	return NULL;
}

static void * resizeex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *tablex, float *tabley)
{
	int x, y, x0, x1;
	int width, height;

	int ix, iy, ic;
	float fx, fy, fc;

	float u, v, vv, uu;
	float uuvv, uuv, vvu, vu;

	IMGRGB *ppixel_dst = NULL;
	IMGRGB *ppixel_src0 = NULL; // this line
	IMGRGB *ppixel_src1 = NULL; // bottom line

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		fy = tabley[y];
		iy = _float2int(fy);

		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[y][0]));
		ppixel_src0 = (IMGRGB *)(&(pimgsrc->matrix[iy][0]));
		ppixel_src1 = fy + 1 <= pimgsrc->ey ? (IMGRGB *)(&(pimgsrc->matrix[iy+1][0])) : ppixel_src0;
		
		// 双线内插值
		v = fy - iy; vv = 1 - v;
		for (x = 0; x < width; x++) {
			fx = tablex[x];
			ix = _float2int(fx);

			u = fx - ix; uu = 1 - u;
			uuvv = uu*vv; vu = v*u;
			uuv = uu*v; vvu = vv*u;

			x0 = ix; x1 = x0 + 1; if (x1 >(int)pimgsrc->ex) x1 = x0;

			// f(i,j)*(1-u)*(1-v) + f(i,j+1)*(1-u)*v + f(i+1,j)*u*(1-v) + f(i+1,j+1)*u*v
			fc = ppixel_src0[x0].R*uuvv + ppixel_src1[x0].R*uuv + ppixel_src0[x1].R*vvu + ppixel_src1[x1].R*vu; ic = _float2int(fc); ppixel_dst[x].R = (unsigned char)ic;
			fc = ppixel_src0[x0].G*uuvv + ppixel_src1[x0].G*uuv + ppixel_src0[x1].G*vvu + ppixel_src1[x1].G*vu; ic = _float2int(fc); ppixel_dst[x].G = (unsigned char)ic;
			fc = ppixel_src0[x0].B*uuvv + ppixel_src1[x0].B*uuv + ppixel_src0[x1].B*vvu + ppixel_src1[x1].B*vu; ic = _float2int(fc); ppixel_dst[x].B = (unsigned char)ic;
		}
	}

	return NULL;
}

static void * resizeex32bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *tablex, float *tabley)
{
	int x, y, x0, x1;
	int width, height;

	int ix, iy, ic;
	float fx, fy, fc;

	float u, v, vv, uu;
	float uuvv, uuv, vvu, vu;

	IMGRGBA *ppixel_dst = NULL;
	IMGRGBA *ppixel_src0 = NULL; // this line
	IMGRGBA *ppixel_src1 = NULL; // bottom line

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	for (y = 0; y < height; y++) {
		fy = tabley[y];
		iy = _float2int(fy);

		ppixel_dst = (IMGRGBA *)(&(pimgdst->matrix[y][0]));
		ppixel_src0 = (IMGRGBA *)(&(pimgsrc->matrix[iy][0]));
		ppixel_src1 = fy + 1 <= pimgsrc->ey ? (IMGRGBA *)(&(pimgsrc->matrix[iy+1][0])) : ppixel_src0;
		
		// 双线内插值
		v = fy - iy; vv = 1 - v;
		for (x = 0; x < width; x++) {
			fx = tablex[x];
			ix = _float2int(fx);

			u = fx - ix; uu = 1 - u;
			uuvv = uu*vv; vu = v*u;
			uuv = uu*v; vvu = vv*u;

			x0 = ix; x1 = x0 + 1; if (x1 >(int)pimgsrc->ex) x1 = x0;

			// f(i,j)*(1-u)*(1-v) + f(i,j+1)*(1-u)*v + f(i+1,j)*u*(1-v) + f(i+1,j+1)*u*v
			fc = ppixel_src0[x0].R*uuvv + ppixel_src1[x0].R*uuv + ppixel_src0[x1].R*vvu + ppixel_src1[x1].R*vu; ic = _float2int(fc); ppixel_dst[x].R = (unsigned char)ic;
			fc = ppixel_src0[x0].G*uuvv + ppixel_src1[x0].G*uuv + ppixel_src0[x1].G*vvu + ppixel_src1[x1].G*vu; ic = _float2int(fc); ppixel_dst[x].G = (unsigned char)ic;
			fc = ppixel_src0[x0].B*uuvv + ppixel_src1[x0].B*uuv + ppixel_src0[x1].B*vvu + ppixel_src1[x1].B*vu; ic = _float2int(fc); ppixel_dst[x].B = (unsigned char)ic;
			fc = ppixel_src0[x0].A*uuvv + ppixel_src1[x0].A*uuv + ppixel_src0[x1].A*vvu + ppixel_src1[x1].A*vu; ic = _float2int(fc); ppixel_dst[x].A = (unsigned char)ic;
		}
	}

	return NULL;
}

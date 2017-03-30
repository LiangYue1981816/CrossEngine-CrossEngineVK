/////////////////////////////////////////////////////////////////////
// rotate.c
//
// 功  能: 旋转影像
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         09:01 2004-12-19
//         11:39 2005-06-19
//         12:01 2007-01-07
//         20:37 2007-01-07
//         09:12 2015-01-14
//===================================================================

#include "_image.h"

static void * rotate1bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy);
static void * rotate8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy);
static void * rotate24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Rotate
//
// 功  能: 创建临时影像
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         float   const fradian 旋转弧度
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Rotate(IMAGE * const pimgsrc, IMAGE * const pimgdst, float const fradian)
{
	int rcode = NO_ERROR;

	int i, width, height;
	int centerx, centery;

	float fcenter;

	float cosa, sina;
	float *sinx = NULL, *siny = NULL;
	float *cosx = NULL, *cosy = NULL;

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}

	//
	// 1. 分配查找表存储
	//
	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);
	fcenter = width/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = height/2.0f + 0.5f; centery = _float2int(fcenter);

	sina = (float)sin(-fradian);
	cosa = (float)cos(-fradian);

	sinx = (float *)calloc(width,  sizeof(float));
	siny = (float *)calloc(height, sizeof(float));
	cosx = (float *)calloc(width,  sizeof(float));
	cosy = (float *)calloc(height, sizeof(float));
	if (NULL == sinx || NULL == siny || NULL == cosx || NULL == cosy) {
		rcode = ERR_MEM; goto RET;
	}

	//
	// 2. 计算目标影像到原影像反向旋转查找表
	//
	// 带坐标变换（屏幕坐标系->直角坐标系）的查找表
	for (i = 0; i < width; i++) {
		sinx[i] = (float)(sina*(i-centerx));
		cosx[i] = (float)(cosa*(i-centerx));
	}
	for (i = 0; i < height; i++) {
		siny[i] = (float)(sina*(centery-i));
		cosy[i] = (float)(cosa*(centery-i));
	}

	//
	// 3. 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1: rotate1bit(pimgsrc, pimgdst, sinx, siny, cosx, cosy); break;
	case  8: rotate8bit(pimgsrc, pimgdst, sinx, siny, cosx, cosy); break;
	case 24: rotate24bit(pimgsrc, pimgdst, sinx, siny, cosx, cosy); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	if (cosx) free(cosx);
	if (cosy) free(cosy);
	if (sinx) free(sinx);
	if (siny) free(siny);
	return rcode;
}

static void * rotate1bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy)
{
	int x, y;
	int width, height;

	int xdstbit;

	int centerx, centery;
	unsigned int srcx, srcy;

	float fx, fy, fcenter;

	unsigned char  *ppixel_dst = NULL;
	unsigned char **ppixel_src = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	fcenter = pimgsrc->sx + IMAGE_AREA_WIDTH(pimgsrc)/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = pimgsrc->sy + IMAGE_AREA_HEIGHT(pimgsrc)/2.0f + 0.5f; centery = _float2int(fcenter);

	ppixel_src = pimgsrc->matrix;

	for (y = 0; y < height; y++) {
		ppixel_dst = (unsigned char *)(&(pimgdst->matrix[y][0]));

		xdstbit = 0;
		for (x = 0; x < width; x++) {
			fx = centerx + (cosx[x] - siny[y]) + 0.5f; srcx = _float2int(fx);
			fy = centery - (cosy[y] + sinx[x]) + 0.5f; srcy = _float2int(fy);

			if (srcx >= pimgsrc->sx && srcx <= pimgsrc->ex && // 水平方向在处理区域内
				srcy >= pimgsrc->sy && srcy <= pimgsrc->ey) { // 垂直方向在处理区域内
				if (0 == (ppixel_src[srcy][srcx/8] & offsetbits1[srcx%8]))
					*ppixel_dst &= offsetbits0[xdstbit]; // 设置为0
				else
					*ppixel_dst |= offsetbits1[xdstbit]; // 设置为1
			}

			xdstbit++; if (8 == xdstbit) { ppixel_dst++; xdstbit = 0; }
		}
	}

	return NULL;
}

static void * rotate8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy)
{
	int x, y;
	int width, height;

	int centerx, centery;
	unsigned int srcx, srcy;

	float fx, fy, fcenter;

	IMGGRAY  *ppixel_dst = NULL;
	IMGGRAY **ppixel_src = NULL;

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	fcenter = pimgsrc->sx + IMAGE_AREA_WIDTH(pimgsrc)/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = pimgsrc->sy + IMAGE_AREA_HEIGHT(pimgsrc)/2.0f + 0.5f; centery = _float2int(fcenter);

	ppixel_src = (IMGGRAY **)(pimgsrc->matrix);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0]));

		for (x = 0; x < width; x++) {
			fx = centerx + (cosx[x] - siny[y]) + 0.5f; srcx = _float2int(fx);
			fy = centery - (cosy[y] + sinx[x]) + 0.5f; srcy = _float2int(fy);

			if (srcx >= pimgsrc->sx && srcx <= pimgsrc->ex && // 水平方向在处理区域内
				srcy >= pimgsrc->sy && srcy <= pimgsrc->ey)   // 垂直方向在处理区域内
				ppixel_dst[x] = ppixel_src[srcy][srcx];
		}
	}

	return NULL;
}

static void * rotate24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy)
{
	int x, y;
	int width, height;

	int centerx, centery;
	unsigned int srcx, srcy;

	float fx, fy, fcenter;

	IMGRGB  *ppixel_dst = NULL;
	IMGRGB **ppixel_src = NULL;

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	fcenter = pimgsrc->sx + IMAGE_AREA_WIDTH(pimgsrc)/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = pimgsrc->sy + IMAGE_AREA_HEIGHT(pimgsrc)/2.0f + 0.5f; centery = _float2int(fcenter);

	ppixel_src = (IMGRGB**)(pimgsrc->matrix);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[y][0]));

		for (x = 0; x < width; x++) {
			fx = centerx + (cosx[x] - siny[y]) + 0.5f; srcx = _float2int(fx);
			fy = centery - (cosy[y] + sinx[x]) + 0.5f; srcy = _float2int(fy);

			if (srcx >= pimgsrc->sx && srcx <= pimgsrc->ex && // 水平方向在处理区域内
				srcy >= pimgsrc->sy && srcy <= pimgsrc->ey)   // 垂直方向在处理区域内
				ppixel_dst[x] = ppixel_src[srcy][srcx];
		}
	}

	return NULL;
}

static void * rotateex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy);
static void * rotateex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_RotateEx
//
// 功  能: 创建临时影像
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         float   const fradian 旋转弧度
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_RotateEx(IMAGE * const pimgsrc, IMAGE * const pimgdst, float const fradian)
{
	int rcode = NO_ERROR;

	int i, width, height;
	int centerx, centery;

	float fcenter;

	float cosa, sina;
	float *sinx = NULL, *siny = NULL;
	float *cosx = NULL, *cosy = NULL;

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}

	//
	// 1. 分配查找表存储
	//
	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);
	fcenter = width/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = height/2.0f + 0.5f; centery = _float2int(fcenter);

	sina = (float)sin(-fradian);
	cosa = (float)cos(-fradian);

	sinx = (float *)calloc(width,  sizeof(float));
	siny = (float *)calloc(height, sizeof(float));
	cosx = (float *)calloc(width,  sizeof(float));
	cosy = (float *)calloc(height, sizeof(float));
	if (NULL == sinx || NULL == siny || NULL == cosx || NULL == cosy) {
		rcode = ERR_MEM; goto RET;
	}

	//
	// 2. 计算目标影像到原影像反向旋转查找表
	//
	// 带坐标变换（屏幕坐标系->直角坐标系）的查找表
	for (i = 0; i < width; i++) {
		sinx[i] = (float)(sina*(i-centerx));
		cosx[i] = (float)(cosa*(i-centerx));
	}
	for (i = 0; i < height; i++) {
		siny[i] = (float)(sina*(centery-i));
		cosy[i] = (float)(cosa*(centery-i));
	}

	//
	// 3. 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: rotateex8bit(pimgsrc, pimgdst, sinx, siny, cosx, cosy); break;
	case 24: rotateex24bit(pimgsrc, pimgdst, sinx, siny, cosx, cosy); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	if (cosx) free(cosx);
	if (cosy) free(cosy);
	if (sinx) free(sinx);
	if (siny) free(siny);
	return rcode;
}

static void * rotateex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy)
{
	int x, y;
	int width, height;

	int centerx, centery;
	unsigned int srcx, srcy;
	unsigned int x0, x1, y0, y1;

	int ic;
	float fc, fcenter;

	float u, v, vv, uu;

	IMGGRAY *ppixel_dst = NULL;
	IMGGRAY *ppixel_src0 = NULL; // this line
	IMGGRAY *ppixel_src1 = NULL; // bottom line

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	fcenter = pimgsrc->sx + IMAGE_AREA_WIDTH(pimgsrc)/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = pimgsrc->sy + IMAGE_AREA_HEIGHT(pimgsrc)/2.0f + 0.5f; centery = _float2int(fcenter);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0]));

		for (x = 0; x < width; x++) {
			u = centerx + cosx[x] - siny[y]; srcx = _float2int(u);
			v = centery - cosy[y] - sinx[x]; srcy = _float2int(v);
			u = u - srcx; uu = 1 - u;
			v = v - srcy; vv = 1 - v;

			if (srcx >= pimgsrc->sx && srcx <= pimgsrc->ex && // 水平方向在处理区域内
				srcy >= pimgsrc->sy && srcy <= pimgsrc->ey) { // 垂直方向在处理区域内
				x0 = srcx; x1 = x0 + 1; if (x1 > pimgsrc->ex) x1 = x0;
				y0 = srcy; y1 = y0 + 1; if (y1 > pimgsrc->ey) y1 = y0;

				ppixel_src0 = (IMGGRAY *)(&(pimgsrc->matrix[y0][0]));
				ppixel_src1 = (IMGGRAY *)(&(pimgsrc->matrix[y1][0]));

				// f(i,j)*(1-u)*(1-v) + f(i,j+1)*(1-u)*v + f(i+1,j)*u*(1-v) + f(i+1,j+1)*u*v
				fc = ppixel_src0[x0]*uu*vv + ppixel_src1[x0]*uu*v + ppixel_src0[x1]*vv*u + ppixel_src1[x1]*v*u; ic = _float2int(fc); ppixel_dst[x] = (IMGGRAY)ic;
			}
		}
	}

	return NULL;
}

static void * rotateex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float *sinx, float *siny, float *cosx, float *cosy)
{
	int x, y;
	int width, height;

	int centerx, centery;
	unsigned int srcx, srcy;
	unsigned int x0, x1, y0, y1;

	int ic;
	float fc, fcenter;

	float u, v, vv, uu;
	float uuvv, uuv, vvu, vu;

	IMGRGB *ppixel_dst = NULL;
	IMGRGB *ppixel_src00 = NULL, *ppixel_src01 = NULL; // this line
	IMGRGB *ppixel_src10 = NULL, *ppixel_src11 = NULL; // bottom line

	width = IMAGE_WIDTH(pimgdst);
	height = IMAGE_HEIGHT(pimgdst);

	fcenter = pimgsrc->sx + IMAGE_AREA_WIDTH(pimgsrc)/2.0f  + 0.5f; centerx = _float2int(fcenter);
	fcenter = pimgsrc->sy + IMAGE_AREA_HEIGHT(pimgsrc)/2.0f + 0.5f; centery = _float2int(fcenter);

	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[y][0]));

		for (x = 0; x < width; x++) {
			u = centerx + cosx[x] - siny[y]; srcx = _float2int(u);
			v = centery - cosy[y] - sinx[x]; srcy = _float2int(v);

			u = u - srcx; uu = 1 - u;
			v = v - srcy; vv = 1 - v;
			uuvv = uu*vv; vu = v*u;
			uuv = uu*v; vvu = vv*u;

			if (srcx >= pimgsrc->sx && srcx <= pimgsrc->ex && // 水平方向在处理区域内
				srcy >= pimgsrc->sy && srcy <= pimgsrc->ey) { // 垂直方向在处理区域内
				x0 = srcx; x1 = x0 + 1; if (x1 > pimgsrc->ex) x1 = x0;
				y0 = srcy; y1 = y0 + 1; if (y1 > pimgsrc->ey) y1 = y0;
				x1 = x1-x0;x0 = x0*3;

				ppixel_src00 = (IMGRGB *)(&(pimgsrc->matrix[y0][x0]));
				ppixel_src10 = (IMGRGB *)(&(pimgsrc->matrix[y1][x0]));
				ppixel_src01 = ppixel_src00 + x1;
				ppixel_src11 = ppixel_src10 + x1;

				// f(i,j)*(1-u)*(1-v) + f(i,j+1)*(1-u)*v + f(i+1,j)*u*(1-v) + f(i+1,j+1)*u*v
				fc = ppixel_src00->R*uuvv + ppixel_src10->R*uuv + ppixel_src01->R*vvu + ppixel_src11->R*vu; ic = _float2int(fc); ppixel_dst[x].R = (unsigned char)ic;
				fc = ppixel_src00->G*uuvv + ppixel_src10->G*uuv + ppixel_src01->G*vvu + ppixel_src11->G*vu; ic = _float2int(fc); ppixel_dst[x].G = (unsigned char)ic;
				fc = ppixel_src00->B*uuvv + ppixel_src10->B*uuv + ppixel_src01->B*vvu + ppixel_src11->B*vu; ic = _float2int(fc); ppixel_dst[x].B = (unsigned char)ic;
			}
		}
	}

	return NULL;
}

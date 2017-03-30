/////////////////////////////////////////////////////////////////////
// blur.c
//
// ��  ��: ģ��
//
// ��  ��: 11:52 2004-12-26
// ��  ��: 18:12 2005-05-16
//         12:23 2005-06-19
//         14:16 2007-01-09
//         14:48 2015-01-13
//===================================================================

#include "_image.h"

// �������
// 1, 1, 1,
// 1, 1, 1,
// 1, 1, 1, 9
static void * blur8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);
static void * blur24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_Blur
//
// ��  ��: ģ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_Blur(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // Ҫ��Ӱ�������򲻵�С�� 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. ���ݲ�ͬɫ��ֱ���ò�ͬ����
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: blur8bit(pimgsrc, pimgdst, &imgwrk); break;
	case 24: blur24bit(pimgsrc, pimgdst, &imgwrk); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * blur8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src_top = NULL;
	IMGGRAY *ppixel_src_ths = NULL;
	IMGGRAY *ppixel_src_btm = NULL;
	IMGGRAY *ppixel_dst_ths = NULL;
	
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
		ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
		ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
		ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			ppixel_dst_ths[x] =
				(ppixel_src_top[x-1] + ppixel_src_top[x] + ppixel_src_top[x+1] +
				 ppixel_src_ths[x-1] + ppixel_src_ths[x] + ppixel_src_ths[x+1] +
				 ppixel_src_btm[x-1] + ppixel_src_btm[x] + ppixel_src_btm[x+1])/9;
		}
	}

	return NULL;
}

static void * blur24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src_top = NULL;
	IMGRGB *ppixel_src_ths = NULL;
	IMGRGB *ppixel_src_btm = NULL;
	IMGRGB *ppixel_dst_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx*3]));
		ppixel_src_ths = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx*3]));
		ppixel_src_btm = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx*3]));
		ppixel_dst_ths = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			ppixel_dst_ths[x].R =
				(ppixel_src_top[x-1].R + ppixel_src_top[x].R + ppixel_src_top[x+1].R +
				 ppixel_src_ths[x-1].R + ppixel_src_ths[x].R + ppixel_src_ths[x+1].R +
				 ppixel_src_btm[x-1].R + ppixel_src_btm[x].R + ppixel_src_btm[x+1].R)/9;

			ppixel_dst_ths[x].G =
				(ppixel_src_top[x-1].G + ppixel_src_top[x].G + ppixel_src_top[x+1].G +
				 ppixel_src_ths[x-1].G + ppixel_src_ths[x].G + ppixel_src_ths[x+1].G +
				 ppixel_src_btm[x-1].G + ppixel_src_btm[x].G + ppixel_src_btm[x+1].G)/9;

			ppixel_dst_ths[x].B =
				(ppixel_src_top[x-1].B + ppixel_src_top[x].B + ppixel_src_top[x+1].B +
				 ppixel_src_ths[x-1].B + ppixel_src_ths[x].B + ppixel_src_ths[x+1].B +
				 ppixel_src_btm[x-1].B + ppixel_src_btm[x].B + ppixel_src_btm[x+1].B)/9;
		}
	}

	return NULL;
}


// ��˹ģ��
// �������
// 1, 2, 1,
// 2, 4, 2,
// 1, 2, 1, 16
static void * gaussblur8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);
static void * gaussblur24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_GaussBlur
//
// ��  ��: ��˹ģ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_GaussBlur(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // Ҫ��Ӱ�������򲻵�С�� 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. ���ݲ�ͬɫ��ֱ���ò�ͬ����
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: gaussblur8bit(pimgsrc, pimgdst, &imgwrk); break;
	case 24: gaussblur24bit(pimgsrc, pimgdst, &imgwrk); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * gaussblur8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src_top = NULL;
	IMGGRAY *ppixel_src_ths = NULL;
	IMGGRAY *ppixel_src_btm = NULL;
	IMGGRAY *ppixel_dst_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
		ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
		ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
		ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			ppixel_dst_ths[x] =
				(  ppixel_src_top[x-1] + 2*ppixel_src_top[x] +   ppixel_src_top[x+1] +
				 2*ppixel_src_ths[x-1] + 4*ppixel_src_ths[x] + 2*ppixel_src_ths[x+1] +
				   ppixel_src_btm[x-1] + 2*ppixel_src_btm[x] +   ppixel_src_btm[x+1])/16;
		}
	}

	return NULL;
}

static void * gaussblur24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src_top = NULL;
	IMGRGB *ppixel_src_ths = NULL;
	IMGRGB *ppixel_src_btm = NULL;
	IMGRGB *ppixel_dst_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx*3]));
		ppixel_src_ths = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx*3]));
		ppixel_src_btm = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx*3]));
		ppixel_dst_ths = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			ppixel_dst_ths[x].R =
				(  ppixel_src_top[x-1].R + 2*ppixel_src_top[x].R +   ppixel_src_top[x+1].R +
				 2*ppixel_src_ths[x-1].R + 4*ppixel_src_ths[x].R + 2*ppixel_src_ths[x+1].R +
				   ppixel_src_btm[x-1].R + 2*ppixel_src_btm[x].R +   ppixel_src_btm[x+1].R)/16;

			ppixel_dst_ths[x].G =
				(  ppixel_src_top[x-1].G + 2*ppixel_src_top[x].G +   ppixel_src_top[x+1].G +
				 2*ppixel_src_ths[x-1].G + 4*ppixel_src_ths[x].G + 2*ppixel_src_ths[x+1].G +
				   ppixel_src_btm[x-1].G + 2*ppixel_src_btm[x].G +   ppixel_src_btm[x+1].G)/16;

			ppixel_dst_ths[x].B =
				(  ppixel_src_top[x-1].B + 2*ppixel_src_top[x].B +   ppixel_src_top[x+1].B +
				 2*ppixel_src_ths[x-1].B + 4*ppixel_src_ths[x].B + 2*ppixel_src_ths[x+1].B +
				   ppixel_src_btm[x-1].B + 2*ppixel_src_btm[x].B +   ppixel_src_btm[x+1].B)/16;
		}
	}

	return NULL;
}


// ��������˹ģ��
static int creategausskernel(float ** const kernel, int * const cw, float const sigma); // ���ɸ�˹�˺���
static void * gaussblurex8bit(IMAGE *pimgdst, IMAGE *pimgwrk0, IMAGE *pimgwrk1, int cw, int *kernel);
static void * gaussblurex24bit(IMAGE *pimgdst, IMAGE *pimgwrk0, IMAGE *pimgwrk1, int cw, int *kernel);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_GaussBlurEx
//
// ��  ��: ��չ��˹ģ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//         float   const sigma   ģ���̶�
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_GaussBlurEx(IMAGE * const pimgsrc, IMAGE * const pimgdst, float const sigma)
{
	int rcode = NO_ERROR;

	int i, cw;

	int ikernel, *kernelex = NULL;
	float fkernel, *kernel = NULL;

	IMAGE imgwrk0;
	IMAGE imgwrk1;
	IMAGE_ZeroImage(&imgwrk0);
	IMAGE_ZeroImage(&imgwrk1);

	//
	// 0. ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. ���ɸ�˹��
	//
	rcode = creategausskernel(&kernel, &cw, sigma);
	if (rcode != NO_ERROR) goto RET;
	
	if (IMAGE_AREA_WIDTH(pimgsrc) < cw || IMAGE_AREA_HEIGHT(pimgsrc) < cw) {
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 2. ���ݲ�ͬɫ��ֱ���ò�ͬ����
	//
	kernelex = (int *)calloc(cw, sizeof(int));
	if (NULL == kernelex) { rcode = ERR_MEM; goto RET; }
	
	for (i = 0; i < cw; i++) {
		fkernel = kernel[i]*1000.0f;
		ikernel = _float2int(fkernel);
		kernelex[i] = ikernel;
	}
	
	rcode = _makeworkimage(pimgsrc, &imgwrk0, cw);
	if (rcode != NO_ERROR) goto RET;

	rcode = _makeworkimage(pimgsrc, &imgwrk1, cw);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: gaussblurex8bit(pimgdst, &imgwrk0, &imgwrk1, cw, kernelex); break;
	case 24: gaussblurex24bit(pimgdst, &imgwrk0, &imgwrk1, cw, kernelex); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk0);
	IMAGE_FreeImage(&imgwrk1);
	if (kernel) free(kernel);
	if (kernelex) free(kernelex);
	return rcode;
}

static int creategausskernel(float ** const kernel, int * const cw, float const sigma)
{
	int rcode = NO_ERROR;
	
	int i, cn;
	float k, sum, alpha, sigmainv;
	
	//
	// 0. ������ȫ���
	//
	assert(cw);
	assert(kernel);
	assert(NULL == *kernel);
	if (NULL == kernel || NULL != *kernel || NULL == cw || 0.0f >= sigma)
		return ERR_PARAMETER;
	
	//
	// 1. ���㴰�ڳߴ����洢
	//
	alpha = (float)ceil(2.5f*sigma);
	cn  = _float2int(alpha);
	*cw = 2*cn+1;
	*kernel  = (float *)calloc((*cw), sizeof(float));
	if (NULL == *kernel) return ERR_MEM;
	
	//
	// 2. �����˹��
	//
	k = (float)(1.0f/sqrt(2*PI)*sigma);
	sigmainv = 1.0f/(sigma*sigma);
	
	sum = 0.0f;
	for (i = -cn; i <= cn; i++) {
		(*kernel)[i+cn] = (float)exp(-0.5f*i*i*sigmainv)*k;
		sum += (*kernel)[i+cn];
	}
	
	for (i = 0; i < (*cw); i++) {
		(*kernel)[i] /= sum;
	}
	
	return rcode;
}

static void * gaussblurex8bit(IMAGE *pimgdst, IMAGE *pimgwrk0, IMAGE *pimgwrk1, int cw, int *kernel)
{
	int x, y, xx, yy;
	int width, height;
	
	int i, wx;
	int weight, total;

	IMGGRAY *ppixel_dst_ths = NULL;
	IMGGRAY *ppixel_wrk0_ths = NULL;
	IMGGRAY *ppixel_wrk1_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgdst);
	height = IMAGE_AREA_HEIGHT(pimgdst);
	
	// �������
	weight = 0; for (i = 0; i < cw; i++) weight += kernel[i];
	wx = cw/2;
	
	// �׶�0: ������ wrk0 => wrk1
	for (y = 0; y < height; y++) {
		ppixel_wrk0_ths = (IMGGRAY *)(&(pimgwrk0->matrix[pimgwrk0->sy+y][pimgwrk0->sx]));
		ppixel_wrk1_ths = (IMGGRAY *)(&(pimgwrk1->matrix[pimgwrk1->sy+y][pimgwrk1->sx]));

		for (x = 0; x < width; x++) {
			total = 0;
			for (xx = -wx; xx <= wx; xx++) {
				total += *(ppixel_wrk0_ths+xx)*kernel[xx+wx];
			}
			*ppixel_wrk1_ths = (IMGGRAY)(total/weight);
			
			ppixel_wrk0_ths++;
			ppixel_wrk1_ths++;
		}
	}

	// �׶�1: ������ wrk1 => dst
	for (x = 0; x < width; x++) {
		ppixel_wrk1_ths = (IMGGRAY *)(&(pimgwrk1->matrix[pimgwrk1->sy][pimgwrk1->sx+x]));
		ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy][pimgdst->sx+x]));
		
		for (y = 0; y < height; y++) {
			total = 0;
			for (yy = -wx; yy <= wx; yy++) {
				total += *(ppixel_wrk1_ths - yy*pimgwrk1->buffWidth)*kernel[yy+wx];
			}
			*ppixel_dst_ths = (IMGGRAY)(total/weight);
			
			ppixel_wrk1_ths -= pimgwrk1->buffWidth;
			ppixel_dst_ths -= pimgdst->buffWidth;
		}
	}

	return NULL;
}

static void * gaussblurex24bit(IMAGE *pimgdst, IMAGE *pimgwrk0, IMAGE *pimgwrk1, int cw, int *kernel)
{
	int x, y, xx, yy;
	int width, height;
	
	int i, wx;
	int weight, w;
	int totalR, totalG, totalB;

	IMGRGB *ppixel_tmp_ths = NULL;
	IMGRGB *ppixel_dst_ths = NULL;
	IMGRGB *ppixel_wrk0_ths = NULL;
	IMGRGB *ppixel_wrk1_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgdst);
	height = IMAGE_AREA_HEIGHT(pimgdst);
	
	// �������
	weight = 0; for (i = 0; i < cw; i++) weight += kernel[i];
	wx = cw/2;
	
	// �׶�0: ������ wrk0 => wrk1
	for (y = 0; y < height; y++) {
		ppixel_wrk0_ths = (IMGRGB *)(&(pimgwrk0->matrix[pimgwrk0->sy+y][pimgwrk0->sx*3]));
		ppixel_wrk1_ths = (IMGRGB *)(&(pimgwrk1->matrix[pimgwrk1->sy+y][pimgwrk1->sx*3]));

		for (x = 0; x < width; x++) {
			totalR = 0;
			totalG = 0;
			totalB = 0;
			for (xx = -wx; xx <= wx; xx++) {
				w = kernel[xx+wx];
				ppixel_tmp_ths = ppixel_wrk0_ths+xx;
				
				totalR += ppixel_tmp_ths->R*w;
				totalG += ppixel_tmp_ths->G*w;
				totalB += ppixel_tmp_ths->B*w;
			}
			ppixel_wrk1_ths->R = (unsigned char)(totalR/weight);
			ppixel_wrk1_ths->G = (unsigned char)(totalG/weight);
			ppixel_wrk1_ths->B = (unsigned char)(totalB/weight);
			
			ppixel_wrk0_ths++;
			ppixel_wrk1_ths++;
		}
	}
	
	// �׶�1: ������ wrk1 => dst
	for (x = 0; x < width; x++) {
		ppixel_wrk1_ths = (IMGRGB *)(&(pimgwrk1->matrix[pimgwrk1->sy][(pimgwrk1->sx+x)*3]));
		ppixel_dst_ths = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy][(pimgdst->sx+x)*3]));
		
		for (y = 0; y < height; y++) {
			totalR = 0;
			totalG = 0;
			totalB = 0;
			for (yy = -wx; yy <= wx; yy++) {
				w = kernel[yy+wx];
				ppixel_tmp_ths = (IMGRGB *)((IMGGRAY *)ppixel_wrk1_ths - yy*pimgwrk1->buffWidth);
				
				totalR += ppixel_tmp_ths->R*w;
				totalG += ppixel_tmp_ths->G*w;
				totalB += ppixel_tmp_ths->B*w;
			}
			ppixel_dst_ths->R = (unsigned char)(totalR/weight);
			ppixel_dst_ths->G = (unsigned char)(totalG/weight);
			ppixel_dst_ths->B = (unsigned char)(totalB/weight);
			
			ppixel_wrk1_ths = (IMGRGB *)((unsigned char *)ppixel_wrk1_ths - pimgwrk1->buffWidth);
			ppixel_dst_ths = (IMGRGB *)((unsigned char *)ppixel_dst_ths - pimgdst->buffWidth);
		}
	}

	return NULL;
}

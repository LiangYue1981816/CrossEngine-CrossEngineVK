/////////////////////////////////////////////////////////////////////
// imagefft.c
//
// ��  ��: Ӱ����Ҷ�任
//
// ��  ��: 20:26 2005-1-2
//         12:23 2005-6-19
//         20:37 2005-7-11
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_DFT
//
// ��  ��: ���ٸ���Ҷ�任
//
// ��  ��: IMAGE   * const pimgsrc     ԴӰ��
//         COMPLEX ** cosnt ppdata      �������ݻ���
//         int        const powerwidth  ���2���ݴ�
//         int        const powerheight �ߵ�2���ݴ�
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_DFT(IMAGE * const pimgsrc, COMPLEX ** const ppdata, int const powerwidth, int const powerheight)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	IMGGRAY *ppixel_src = NULL;
	COMPLEX *TD = NULL, *FD = NULL;

	//
	// 0. ��ȫ���
	//
	assert(ppdata);
	assert(pimgsrc);
	if (NULL == pimgsrc->data || NULL == ppdata) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != 8) { // Ҫ��ԭӰ�������8bitӰ��
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != (unsigned int)(1<<powerwidth) || IMAGE_AREA_HEIGHT(pimgsrc) != (unsigned int)(1<<powerheight)) { // Ҫ��Ӱ���߱�����2����������
		rcode = ERR_RANGE; goto RET;
	}
	
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	//
	// 1. ����洢
	//
	TD = (COMPLEX *)calloc(width*height, sizeof(COMPLEX));
	FD = (COMPLEX *)calloc(width*height, sizeof(COMPLEX));
	if (NULL == TD || NULL == FD) { rcode = ERR_MEM; goto RET; }
	
	//
	// 2. ����Ҷ�任
	//
	// 2.1. ��ȡӰ������
	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));

		for (x = 0; x < width; x++) {
			TD[y*width + x].re = ppixel_src[x];
		}
	}
	
	// 2.2. �и���Ҷ�任
	for (y = 0; y < height; y++) {
		rcode = _FFT(&TD[y*width], &FD[y*width], powerwidth);
		if (rcode != NO_ERROR) goto RET;
	}
	
	// 2.1. ���и���Ҷ������FDת�ñ��浽TD������
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			TD[x*height+y] = FD[y*width+x];
		}
	}
	
	// 2.3. �и���Ҷ�任
	for (x = 0; x < width; x++) {
		rcode = _FFT(&TD[x*height], &FD[x*height], powerheight);
		if (rcode != NO_ERROR) goto RET;
	}
	
	//
	// 3. �����������Ƶ��������
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ppdata[y][x].re = FD[y*width+x].re;
			ppdata[y][x].im = FD[y*width+x].im;
		}
	}
	
RET:
	if (TD) free(TD);
	if (FD) free(FD);
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_IDFT
//
// ��  ��: ���ٷ�����Ҷ�任
//
// ��  ��: COMPLEX ** cosnt ppdata      �������ݻ���
//         IMAGE   * const pimgsrc     ԴӰ��
//         int        const powerwidth  ���2���ݴ�
//         int        const powerheight �ߵ�2���ݴ�
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_IDFT(COMPLEX ** const ppdata, IMAGE * const pimgdst, int const powerwidth, int const powerheight)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	IMGGRAY *ppixel_dst = NULL;
	COMPLEX *TD = NULL, *FD = NULL;

	//
	// 0. ��ȫ���
	//
	assert(ppdata);
	assert(pimgdst);
	if (NULL == pimgdst->data || NULL == ppdata) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgdst) != 8) { // Ҫ��ԭӰ�������8bitӰ��
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgdst) != (unsigned int)(1<<powerwidth) || IMAGE_AREA_HEIGHT(pimgdst) != (unsigned int)(1<<powerheight)) { // Ҫ��Ӱ���߱�����2����������
		rcode = ERR_RANGE; goto RET;
	}
	
	width  = IMAGE_AREA_WIDTH(pimgdst);
	height = IMAGE_AREA_HEIGHT(pimgdst);

	//
	// 1. ����洢
	//
	TD = (COMPLEX *)calloc(width*height, sizeof(COMPLEX));
	FD = (COMPLEX *)calloc(width*height, sizeof(COMPLEX));
	if (NULL == TD || NULL == FD) { rcode = ERR_MEM; goto RET; }

	//
	// 2. �����븴�����ݸ��Ƶ�Ƶ��洢
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			FD[y*width+x].re = ppdata[y][x].re;
			FD[y*width+x].im = ppdata[y][x].im;
		}
	}

	//
	// 3. ������Ҷ�任
	//
	// 3.1. �з�����Ҷ�任
	for (y = 0; y < height; y++) {
		rcode = _IFFT(&FD[y*width], &TD[y*width], powerwidth);
		if (rcode != NO_ERROR) goto RET;
	}
	
	// 3.2. ���з�����Ҷ������TDת�ñ��浽FD������
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			FD[x*height+y] = TD[y*width+x];
		}
	}
	
	// 3.3. �з�����Ҷ�任
	for (x = 0; x < width; x++) {
		rcode = _IFFT(&FD[x*height], &TD[x*height], powerheight);
		if (rcode != NO_ERROR) goto RET;
	}
	
	//
	// 4. ��������Ӱ��
	//
	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			if (TD[y*width + x].re > 255.0) TD[y*width + x].re = 255.0;
			if (TD[y*width + x].re < 000.0) TD[y*width + x].re = 000.0;
			ppixel_dst[x] = (IMGGRAY)(TD[y*width + x].re);
		}
	}
	
RET:
	if (TD) free(TD);
	if (FD) free(FD);
	return rcode;
}

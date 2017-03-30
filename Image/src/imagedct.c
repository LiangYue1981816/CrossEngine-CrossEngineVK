/////////////////////////////////////////////////////////////////////
// imagedct.c
//
// ��  ��: Ӱ����ɢ���ұ任
//
// ��  ��: 20:26 2005-1-2
//         12:23 2005-6-19
//         20:40 2005-7-11
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_DCT
//
// ��  ��: Ӱ����ɢ���ұ任
//
// ��  ��: IMAGE * const pimgsrc     ԴӰ��
//         float ** cosnt ppdata      ���ݻ���
//         int      const powerwidth  ���2���ݴ�
//         int      const powerheight �ߵ�2���ݴ�
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_DCT(IMAGE * const pimgsrc, float ** const ppdata, int const powerwidth, int const powerheight)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	float *TD = NULL, *FD = NULL;
	IMGGRAY *ppixel_src = NULL;

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
	TD = (float *)calloc(width*height, sizeof(float));
	FD = (float *)calloc(width*height, sizeof(float));
	if (NULL == TD || NULL == FD) { rcode = ERR_MEM; goto RET; }
	
	//
	// 2. ��ɢ���ұ任
	//
	// 2.1. ��ȡӰ������
	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));

		for (x = 0; x < width; x++) {
			TD[y*width + x] = ppixel_src[x];
		}
	}
	
	// 2.2. ����ɢ���ұ任
	for (y = 0; y < height; y++) {
		rcode = _DCT(&TD[y*width], &FD[y*width], powerwidth);
		if (rcode != NO_ERROR) goto RET;
	}
	
	// 2.1. ������ɢ���Ҽ�����FDת�ñ��浽TD������
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			TD[x*height+y] = FD[y*width+x];
		}
	}
	
	// 2.3. ����ɢ���ұ任
	for (x = 0; x < width; x++) {
		rcode = _DCT(&TD[x*height], &FD[x*height], powerheight);
		if (rcode != NO_ERROR) goto RET;
	}
	
	//
	// 3. �����������Ƶ��������
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ppdata[y][x] = FD[y*width+x];
		}
	}
	
RET:
	if (TD) free(TD);
	if (FD) free(FD);
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_IDCT
//
// ��  ��: ���ٷ���ɢ���ұ任
//
// ��  ��: float ** cosnt ppdata      ���ݻ���
//         IMAGE * const pimgsrc     ԴӰ��
//         int      const powerwidth  ���2���ݴ�
//         int      const powerheight �ߵ�2���ݴ�
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_IDCT(float ** const ppdata, IMAGE * const pimgdst, int const powerwidth, int const powerheight)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	float *TD = NULL, *FD = NULL;
	IMGGRAY *ppixel_dst = NULL;

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
	TD = (float *)calloc(width*height, sizeof(float));
	FD = (float *)calloc(width*height, sizeof(float));
	if (NULL == TD || NULL == FD) { rcode = ERR_MEM; goto RET; }

	//
	// 2. �����븴�����ݸ��Ƶ�Ƶ��洢
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			FD[y*width+x] = ppdata[y][x];
		}
	}

	//
	// 3. ������Ҷ�任
	//
	// 3.1. �з���ɢ���ұ任
	for (y = 0; y < height; y++) {
		rcode = _IDCT(&FD[y*width], &TD[y*width], powerwidth);
		if (rcode != NO_ERROR) goto RET;
	}
	
	// 3.2. ���з���ɢ���Ҽ�����TDת�ñ��浽FD������
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			FD[x*height+y] = TD[y*width+x];
		}
	}
	
	// 3.3. �з���ɢ���ұ任
	for (x = 0; x < width; x++) {
		rcode = _IDCT(&FD[x*height], &TD[x*height], powerheight);
		if (rcode != NO_ERROR) goto RET;
	}
	
	//
	// 4. ��������Ӱ��
	//
	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			if (TD[y*width + x] > 255.0) TD[y*width + x] = 255.0;
			if (TD[y*width + x] < 000.0) TD[y*width + x] = 000.0;
			ppixel_dst[x] = (IMGGRAY)(TD[y*width + x]);
		}
	}
	
RET:
	if (TD) free(TD);
	if (FD) free(FD);
	return rcode;
}

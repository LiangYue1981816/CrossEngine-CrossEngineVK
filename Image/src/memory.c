/////////////////////////////////////////////////////////////////////
// memory.c
//
// ��  ��: �ṩӰ��洢������洢������洢�ĳ�ʼ���������Լ��ͷŲ���
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         09:01 2004-12-19
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_AllocImage
//
// ��  ��: ����Ӱ��洢�ռ�
//
// ��  ��: IMAGE * const pimage    Ӱ��ͷָ��
//         int     const nwidth    Ӱ���(in pixel)
//         int     const nheight   Ӱ���(in pixel)
//         int     const nbitcount Ӱ��ɫ��(1, 8, 24, 32)
//
// ����ֵ��int
//
// ע  �⣺ֻ֧�� 1-bit, 8-bit, 16-bit, 24-bit, 32-bit�Ҳ�ʹ��ѹ����ʽ
//===================================================================
EXPORT_C int IMAGE_AllocImage(IMAGE * const pimage, int const nwidth, int const nheight, int const nbitcount)
{
	int rcode = NO_ERROR;

	int i, nheadersize, nbuffersize, nlinesize;
	unsigned char *pbuffer = NULL, **ppline = NULL, *pbody = NULL;

	unsigned long rgb565Mask;

	//
	// 0. ������ȫ���
	//
	assert(pimage);
	if (NULL == pimage) { // Ӱ��ָ����
		return ERR_PARAMETER;
	}
	if (nwidth <= 0 || nheight <= 0) { // �ߴ緶Χ���
		return ERR_RANGE;
	}
	if (NULL != pimage->data || NULL != pimage->matrix || NULL != pimage->bmpInfo) { // Ӱ��δ����ʼ���������Ѿ�������洢
		return ERR_IMAGE;
	}

	//
	// 1. ��ʼ����� IMAGE ��Ϣ
	//
	pimage->sx = 0; pimage->ex = nwidth -1; // Ĭ�Ϻ�����Χ������Ӱ���
	pimage->sy = 0; pimage->ey = nheight-1; // Ĭ��������Χ������Ӱ���
	pimage->buffWidth = ((nwidth*nbitcount+31)/32)*4; // �洢�п������4��������

	//
	// 2. ���� BITMAPINFO
	//
	// 2.1. ���ݲ�ͬ��ɫ����� BITMAPINFO �洢
	switch (nbitcount) {
	case 1: // 1-bit
		//
		// 2.1.1. ����洢(�ڰ�2ɫ��ɫ��)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, (sizeof(BITMAPINFOHEADER) + 2*sizeof(RGBQUAD)));
		if (NULL == pimage->bmpInfo) {	rcode = ERR_MEM; goto RET; }

		//
		// 2.1.2. ������ɫ��(�ڰ�2ɫ)
		//
		pimage->bmpInfo->bmiHeader.biClrUsed      = 2;
		pimage->bmpInfo->bmiHeader.biClrImportant = 2;

		//
		// 2.1.3. ���õ�ɫ��(�ڰ�2ɫ��ɫ��)
		//
		pimage->bmpInfo->bmiColors[0].rgbRed      = 0;
		pimage->bmpInfo->bmiColors[0].rgbGreen    = 0;
		pimage->bmpInfo->bmiColors[0].rgbBlue     = 0;
		pimage->bmpInfo->bmiColors[0].rgbReserved = 0;

		pimage->bmpInfo->bmiColors[1].rgbRed      = 255;
		pimage->bmpInfo->bmiColors[1].rgbGreen    = 255;
		pimage->bmpInfo->bmiColors[1].rgbBlue     = 255;
		pimage->bmpInfo->bmiColors[1].rgbReserved = 255;

		break;
	case 8: // 8-bit
		//
		// 2.1.1. ����洢(256ɫ��ɫ��)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, (sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)));
		if (NULL == pimage->bmpInfo) {	rcode = ERR_MEM; goto RET; }

		//
		// 2.1.2. ������ɫ��(256ɫ��ɫ��)
		//
		pimage->bmpInfo->bmiHeader.biClrUsed      = 256;
		pimage->bmpInfo->bmiHeader.biClrImportant = 256;

		//
		// 2.1.3. ���õ�ɫ��(256ɫ�ҽ׵�ɫ��)
		//
		for (i = 0; i < 256; i++) {
			pimage->bmpInfo->bmiColors[i].rgbRed      = (unsigned char)i;
			pimage->bmpInfo->bmiColors[i].rgbGreen    = (unsigned char)i;
			pimage->bmpInfo->bmiColors[i].rgbBlue     = (unsigned char)i;
			pimage->bmpInfo->bmiColors[i].rgbReserved = (unsigned char)i;
		}

		break;
	case 16: // 16-bit
		//
		// 2.1.1. ����洢(16Bitʹ������)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFOHEADER) + 3*sizeof(RGBQUAD));
		if (!pimage->bmpInfo) { rcode = ERR_MEM; goto RET; }

		//
		// 2.1.2. ���õ�ɫ��
		//
		pimage->bmpInfo->bmiHeader.biClrUsed      = 3;
		pimage->bmpInfo->bmiHeader.biClrImportant = 65536;

		rgb565Mask = 0x0000f800; pimage->bmpInfo->bmiColors[0] = *((RGBQUAD *)(&rgb565Mask));
		rgb565Mask = 0x000007e0; pimage->bmpInfo->bmiColors[1] = *((RGBQUAD *)(&rgb565Mask));
		rgb565Mask = 0x0000001f; pimage->bmpInfo->bmiColors[2] = *((RGBQUAD *)(&rgb565Mask));

		break;
	case 24: // 24-bit
	case 32: // 32-bit
		//
		// 2.1.1. ����洢(���ɫ��ʹ�õ�ɫ��)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, (sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)));
		if (NULL == pimage->bmpInfo) {	rcode = ERR_MEM; goto RET; }

		break;
	default: rcode = ERR_BITCOUNT; goto RET; // �Ƿ�ɫ��
	}

	// 2.2. ��� BITMAPINFO ��Ϣ
	pimage->bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pimage->bmpInfo->bmiHeader.biPlanes = 1;
	pimage->bmpInfo->bmiHeader.biCompression = BI_RGB; // ��ʹ��ѹ����ʽ

	pimage->bmpInfo->bmiHeader.biWidth = nwidth;
	pimage->bmpInfo->bmiHeader.biHeight = nheight;
	pimage->bmpInfo->bmiHeader.biBitCount = (WORD)nbitcount;

	pimage->bmpInfo->bmiHeader.biSizeImage = pimage->buffWidth*nheight;

	//
	// 3. ����Ӱ�����
	//
	// 3.1. ����Ӱ�����ͷ���У���������Ҫ�Ĵ洢����
	nlinesize   = pimage->buffWidth;
	nheadersize = ((nheight*sizeof(void *)+3)/4)*4;
	nbuffersize = nheadersize + nlinesize*nheight;

	// 3.2. ����洢
	pbuffer = (unsigned char *)calloc(nbuffersize, sizeof(unsigned char));
	if (NULL == pbuffer) { rcode = ERR_MEM; goto ERR; }

	// 3.3. ����Ӱ��������ָ��
	//      ע�⣺����ʹ�õ���ķ�ʽ���þ���洢��Ŀ����Ӱ�����ݴ洢��ʽ����һ��
	ppline = (unsigned char **)pbuffer;
	pbody = pbuffer + nbuffersize - nlinesize;
	for (i = 0; i < nheight; i++) { *ppline = pbody; pbody -= nlinesize; ppline++; }

	// 3.4. ����Ӱ�����Ӱ������ָ��
	pimage->matrix = (unsigned char **)pbuffer;
	pimage->data = pbuffer + nheadersize;

	goto RET;
ERR:
	IMAGE_FreeImage(pimage);
RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_FreeImage
//
// ��  ��: �ͷ�Ӱ��洢
//
// ��  ��: IMAGE * const pimage Ӱ��ͷָ��
//
// ����ֵ: void
//
// ע  ��: �ú���ֻ���ͷű� IMAGE_AllocImage �������Ӱ�񣬷�������
//         �ڴ�й©��������
//===================================================================
EXPORT_C void IMAGE_FreeImage(IMAGE * const pimage)
{
	//
	// 0. ������ȫ���
	//
	assert(pimage);

	//
	// 1. �ͷ�Ӱ��洢
	//
	if (pimage) {
		if (pimage->matrix) free(pimage->matrix);
		if (pimage->bmpInfo) free(pimage->bmpInfo);
		IMAGE_ZeroImage(pimage);
	}
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_ZeroImage
//
// ��  ��: ��ʼ��Ӱ��ͷ
//
// ��  ��: IMAGE * const pimage Ӱ��ͷָ��(δ��������������Ѿ��ͷŹ�)
//
// ����ֵ: void
//===================================================================
EXPORT_C void IMAGE_ZeroImage(IMAGE * const pimage)
{
	//
	// 0. ������ȫ���
	//
	assert(pimage);

	//
	// 1. ��ʼ��Ӱ��ͷ�ṹ
	//
	if (pimage) memset(pimage, 0, sizeof(IMAGE));
}



/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_AllocMatrix
//
// ��  ��: ��������ά����洢
//
// ��  ��: int const nrows ��������
//         int cosnt ncols ��������
//         int cosnt nsize ������ÿ��Ԫ����ռ�洢������
//
// ����ֵ: void ** ����ָ�����洢��ָ�룬������洢ʧ�ܣ�����NULL
/////////////////////////////////////////////////////////////////////
EXPORT_C void** IMAGE_AllocMatrix(int const nrows, int const ncols, int const nsize)
{
	int i, nheadersize, nbuffersize, nlinesize;
	unsigned char *pbuffer = NULL, **ppline = NULL, *pbody = NULL;

	//
	// 0. ������ȫ���
	//
	if (nrows < 0 || ncols < 0 || nsize <= 0)
		return NULL;

	//
	// 1. �������ͷ���У���������Ҫ�Ĵ洢����
	//
	nlinesize   = ((ncols*nsize+3)/4)*4;
	nheadersize = ((nrows*sizeof(void *)+3)/4)*4;
	nbuffersize = nheadersize + nlinesize*nrows;

	//
	// 2. ����洢
	//
	pbuffer = (unsigned char *)calloc(nbuffersize, sizeof(unsigned char));
	if (NULL == pbuffer) return NULL;

	//
	// 3. ���þ������ָ��
	//    ע�⣺����ʹ�õ���ķ�ʽ���þ���洢��Ŀ����Ӱ�����ݴ洢��ʽ����һ��
	ppline = (unsigned char **)pbuffer;
	pbody = pbuffer + nbuffersize - nlinesize;
	for (i = 0; i < nrows; i++) { *ppline = pbody; pbody -= nlinesize; ppline++; }

	//
	// 4. ��������ָ���ַ��Ϊ����ָ��
	//
	ppline = (unsigned char **)pbuffer;

	return (void**)ppline;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_FreeMatrix
//
// ��  ��: �ͷž����ά����洢
//
// ��  ��: void ** cosnt pmatrix ����洢ָ��
//
// ����ֵ: void
/////////////////////////////////////////////////////////////////////
EXPORT_C void IMAGE_FreeMatrix(void ** const pmatrix)
{
	//
	// 0. ������ȫ���
	//
	assert(pmatrix);

	//
	// 1. �ͷž���洢
	//
	if (pmatrix) free(pmatrix);
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CopyMatrix
//
// ��  ��: ���ƶ�ά����
//
// ��  ��: void ** const pmtxsrc ԭ����
//         void ** const pmtxdst Ŀ�����
//         int     cosnt nrows   ��������
//         int     cosnt ncols   ��������
//         int     const nsize   Ԫ�ش�С
//
// ����ֵ: int
/////////////////////////////////////////////////////////////////////
EXPORT_C int IMAGE_CopyMatrix(void ** const pmtxsrc, void ** const pmtxdst, int const nrows, int const ncols, int const nsize)
{
	int i;

	//
	// 0. ������ȫ���
	//
	assert(pmtxsrc);
	assert(pmtxdst);
	if (NULL == pmtxsrc || NULL == pmtxdst) { // ����洢��Ч
		return ERR_MATRIX;
	}
	if (nrows < 0 || ncols < 0 || nsize <= 0) { // ���������ΧԽ��
		return ERR_PARAMETER;
	}

	//
	// 1. ���и��ƾ���
	//
	for (i = 0; i < nrows; i++) memcpy(pmtxdst[i], pmtxsrc[i], ncols*nsize);

	return NO_ERROR;
}



static void copyimagearea1bit(IMAGE  * const pimgsrc, IMAGE * const pimgdst);
static void copyimagearea8bit(IMAGE  * const pimgsrc, IMAGE * const pimgdst);
static void copyimagearea24bit(IMAGE * const pimgsrc, IMAGE * const pimgdst);
static void copyimagearea32bit(IMAGE * const pimgsrc, IMAGE * const pimgdst);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CopyImage
//
// ��  ��: ����ȫ�ߴ�Ӱ������
//
// ��  ��: IMAGE * const pimgsrc ԭӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ��int
//===================================================================
EXPORT_C int IMAGE_CopyImage(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	//
	// 0. ������ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	assert(pimgsrc != pimgdst);
	assert(pimgsrc->data != pimgdst->data);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimgsrc) != IMAGE_WIDTH(pimgdst) ||	IMAGE_HEIGHT(pimgsrc) != IMAGE_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ߴ���ͬ
		return ERR_RANGE;
	}

	//
	// 1. ����Ӱ��
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1:
	case  8:
	case 24:
	case 32: memcpy(pimgdst->data, pimgsrc->data, pimgdst->bmpInfo->bmiHeader.biSizeImage); break;
	default: return ERR_BITCOUNT;
	}
	
	//
	// 2. ���ƴ�����������
	//
	pimgdst->sx = pimgsrc->sx; pimgdst->ex = pimgsrc->ex;
	pimgdst->sy = pimgsrc->sy; pimgdst->ey = pimgsrc->ey;

	return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CopyImageArea
//
// ��  ��: ��������Ӱ������
//
// ��  ��: IMAGE * const pimgsrc ԭӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ��int
//===================================================================
EXPORT_C int IMAGE_CopyImageArea(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{	
	//
	// 0. ������ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ
		return ERR_BITCOUNT;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		return ERR_RANGE;
	}

	//
	// 1. ����ɫ��Ĳ�ͬ���ò�ͬ�Ŀ�������
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1: copyimagearea1bit(pimgsrc, pimgdst); break;
	case  8: copyimagearea8bit(pimgsrc, pimgdst); break;
	case 24: copyimagearea24bit(pimgsrc,pimgdst); break;
	case 32: copyimagearea32bit(pimgsrc,pimgdst); break;
	default: return ERR_BITCOUNT;
	}

	return NO_ERROR;
}

void copyimagearea1bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int x, y;
	int width, height;

	int xsrcbit, xdstbit;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		xsrcbit = pimgsrc->sx%8;
		xdstbit = pimgdst->sx%8;

		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx/8]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx/8]);

		for (x = 0; x < width; x++) {
			if (0 == ((*ppixel_src) & offsetbits1[xsrcbit]))
				*ppixel_dst &= offsetbits0[xdstbit]; // ����Ϊ0
			else
				*ppixel_dst |= offsetbits1[xdstbit]; // ����Ϊ1

			xsrcbit++; if (8 == xsrcbit) { ppixel_src++; xsrcbit = 0; }
			xdstbit++; if (8 == xdstbit) { ppixel_dst++; xdstbit = 0; }
		}
	}
}

void copyimagearea8bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc); // ���� 8-bit Ӱ����˵ÿ�и��Ƶ��ֽ�������������
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]);

		memcpy(ppixel_dst, ppixel_src, width);
	}
}

void copyimagearea24bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc)*3; // ���� 24-bit Ӱ����˵ÿ�и��Ƶ��ֽ������������ȵ�3��
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]);

		memcpy(ppixel_dst, ppixel_src, width);
	}
}

static void copyimagearea32bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc)*4; // ���� 32-bit Ӱ����˵ÿ�и��Ƶ��ֽ������������ȵ�4��
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*4]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*4]);

		memcpy(ppixel_dst, ppixel_src, width);
	}
}

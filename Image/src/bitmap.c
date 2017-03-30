/////////////////////////////////////////////////////////////////////
// bitmap.c
//
// ��  ��: �ṩ��*.bmpӰ���I/O��������֧��ѹ��
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//         10:59 2007-01-08
//         20:52 2007-09-17
//===================================================================

#include "_image.h"

static int loadbmpheader(FILE * const pfile, unsigned long * const pdwfilesize, unsigned long * const pdwoffbits, BITMAPINFOHEADER * const pbmih);

//////////////////////////////////////////////////////////////////
//                       λͼ�ṹ:                              //
//         |================================|                   //
//         |          BITMAPFILEHEADER      | 14 byte           //
//         |--------------------------------|                   //
//         |          BITMAPINFOHEADER      | 40 byte           //
//         |--------------------------------|                   //
//         |              RGBQUAD           | n*sizeof(RGBQUAD) //
//         |--------------------------------|                   //
//         |            IMAGE DATA          | n byte            //
//         |               ....             |                   //
//         |               ....             |                   //
//         |               ....             |                   //
//         |================================|                   //
//////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadBmpHeader
//
// ��  ��: ���� .bmp Ӱ���ļ� BITMAPINFOHEADER ����
//
// ��  ��: char             * const pszfilename    �ļ�·��
//         BITMAPINFOHEADER * const pbmpinfoheader ͷָ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadBmpHeader(char * const pszfilename, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. ��ȫ���
	//
	assert(pszfilename);
	assert(pbmpinfoheader);
	if (NULL == pszfilename || NULL == pbmpinfoheader) { // �����������
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. �Զ�����ֻ����ʽ���ļ�
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. ��ȡ�ļ���Ϣ
	//
	rcode = IMAGE_LoadBmpHeaderEx(pfile, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (NULL != pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadBmpHeaderEx
//
// ��  ��: ���� .bmp Ӱ���ļ� BITMAPINFOHEADER ����
//
// ��  ��: FILE             * const pfile          �ļ�ָ��
//         BITMAPINFOHEADER * const pbmpinfoheader ͷָ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadBmpHeaderEx(FILE * const pfile, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	unsigned long dwfilesize, dwoffbits; // dwfilesize = dwoffbits + IMAGE DATA
                                         // dwoffbits  = BITMAPFILEHEADER + BITMAPINFOHEADER + n*RGBQUAD

	//
	// 0. ��ȫ���
	//
	assert(pfile);
	assert(pbmpinfoheader);
	if (NULL == pfile || NULL == pbmpinfoheader) { // �����������
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. ��ȡ�ļ���Ϣ
	//
	rcode = loadbmpheader(pfile, &dwfilesize, &dwoffbits, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadBmp
//
// ��  ��: ���� .bmp Ӱ���ļ�
//
// ��  ��: char  * const pszfilename �ļ�·��
//         IMAGE * const pimage      Ӱ��ͷָ��
//
// ����ֵ��int
//===================================================================
EXPORT_C int IMAGE_LoadBmp(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. ��ȫ���
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // �����������
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL != pimage->data) { // Ӱ��δ����ʼ���������Ѿ�������洢
		rcode = ERR_IMAGE; goto RET;
	};

	//
	// 1. �Զ�����ֻ����ʽ���ļ�
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. ����Bitmap�ļ�
	//
	rcode = IMAGE_LoadBmpEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (NULL != pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadBmpEx
//
// ��  ��: ���� .bmp Ӱ���ļ�
//
// ��  ��: FILE  * const pfile  �ļ�ָ��
//         IMAGE * const pimage Ӱ��ͷָ��
//
// ����ֵ��int
//===================================================================
EXPORT_C int IMAGE_LoadBmpEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	size_t size;
	BITMAPINFOHEADER bmih;
	unsigned long dwoffset, dwfilesize, dwoffbits; // dwfilesize = dwoffbits + IMAGE DATA
                                                   // dwoffbits  = BITMAPFILEHEADER + BITMAPINFOHEADER + n*RGBQUAD

	//
	// 0. ��ȫ���
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // �����������
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL != pimage->data) { // Ӱ��δ����ʼ���������Ѿ�������洢
		rcode = ERR_IMAGE; goto RET;
	}

	// ��õ�ǰ�ļ�ƫ����
	dwoffset = ftell(pfile);

	//
	// 1. ��ȡ bmp �ļ�ͷ�������м��������
	//
	rcode = loadbmpheader(pfile, &dwfilesize, &dwoffbits, &bmih);
	if (rcode != NO_ERROR) goto RET;

	if (1 != bmih.biBitCount &&
		8 != bmih.biBitCount &&
		16!= bmih.biBitCount &&
		24!= bmih.biBitCount &&
		32!= bmih.biBitCount) { // Ӱ���ļ�ɫ����
		rcode = ERR_IO_BITCOUNT; goto RET;
	}
	if (BI_RGB != bmih.biCompression) { // Ӱ���ļ�ѹ�����
		rcode = ERR_IO_COMPRESS; goto RET;
	}

	//
	// 2. ����Ӱ���ļ�ͷ��Ϣ����洢
	//    (ע�⣺ͨ�� IMAGE_AllocImage ����� 8-bit Ӱ��Ĭ���ǻҽ�Ӱ��)
	//
	rcode = IMAGE_AllocImage(pimage, bmih.biWidth, bmih.biHeight, bmih.biBitCount);
	if (rcode != NO_ERROR) goto RET;
	
	//
	// 3. ��ȡ��ɫ����Ϣ
	//
	if (1 == bmih.biBitCount || // 1-bit
		8 == bmih.biBitCount) { // 8-bit
		// 4.1. ��λ��ɫ��λ��
		rcode = fseek(pfile, dwoffset, SEEK_SET);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		rcode = fseek(pfile, sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER), SEEK_CUR);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		// 4.2. ��ȡ��ɫ����Ϣ
		size = dwoffbits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		if (size != fread((void*)(pimage->bmpInfo->bmiColors), 1, size, pfile) ) {
			rcode = ERR_IO_READDATA; goto ERR;
		}
	}

	//
	// 4. ��ȡ������Ϣ
	//
	size = min(dwfilesize-dwoffbits, pimage->bmpInfo->bmiHeader.biSizeImage);
	if (0 != size) {
		// 4.1. ��λ������Ϣλ��
		rcode = fseek(pfile, dwoffset, SEEK_SET);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		rcode = fseek(pfile, dwoffbits, SEEK_CUR);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		// 4.2. ��ȡ������Ϣ
		if (size != fread((void*)(pimage->data), 1, size, pfile) ) {
			rcode = ERR_IO_READDATA; goto ERR;
		}
	}

	//
	// 5. ת��Ϊ�ڲ�����ı�׼Ӱ��
	//
	rcode = _converttostdimage(pimage);
	if (rcode != NO_ERROR) goto ERR;

	goto RET;
ERR:
	IMAGE_FreeImage(pimage);
RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SaveBmp
//
// ��  ��: �����׼��ѹ���� .bmp Ӱ���ļ�
//
// ��  ��: char  * const pszfilename �ļ�·��
//         IMAGE * const pimage      Ӱ��ͷָ��
//
// ����ֵ��int 
//===================================================================
EXPORT_C int IMAGE_SaveBmp(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. ��ȫ���
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // �����������
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL == pimage->data) { // Ӱ��������Ч�Լ��
		rcode = ERR_IMAGE; goto RET;
	}

	//
	// 1. ���ļ�
	//
	pfile = fopen(pszfilename, "wb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. ��ȡBitmpӰ���ļ�
	//
	rcode = IMAGE_SaveBmpEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;
	
RET:
	//
	// 3. �ر��ļ�
	//
	if (NULL != pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SaveBmpEx
//
// ��  ��: �����׼��ѹ���� .bmp Ӱ���ļ�
//
// ��  ��: FILE  * const pfile  �ļ�ָ��
//         IMAGE * const pimage Ӱ��ͷָ��
//
// ����ֵ��int 
//===================================================================
EXPORT_C int IMAGE_SaveBmpEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	size_t size;
	BITMAPFILEHEADER bmfh;

	//
	// 0. ��ȫ���
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // �����������
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL == pimage->data) { // Ӱ��������Ч�Լ��
		rcode = ERR_IMAGE; goto RET;
	}

	//
	// 1. �� bitmap Ӱ������д�� bmp Ӱ���ļ�
	//
	// 1.1.1. ���� BITMAPFILEHEADER �ṹ��Ϣ
	bmfh.bfReserved1 = 0;			// ������0
	bmfh.bfReserved2 = 0;			// ������0
	bmfh.bfType = *((WORD*)"BM");	// λͼ������ "BM"
	bmfh.bfOffBits =				// BITMAPFILEHEADER + BITMAPINFOHEADER + ��ɫ��
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pimage->bmpInfo->bmiHeader.biClrUsed*sizeof(RGBQUAD);
	bmfh.bfSize =					// bmfh.bfOffBits + Image Data Size
		bmfh.bfOffBits + pimage->bmpInfo->bmiHeader.biSizeImage;

	// 1.1.2. д�� BITMAPFILEHEADER
	size = sizeof(BITMAPFILEHEADER); // 14 byte
	if (size != fwrite((void*)(&bmfh), 1, size, pfile)) {
		rcode = ERR_IO_WRITEDATA; goto RET;
	}

	// 1.2. д�� BITMAPINFOHEADER
	size = sizeof(BITMAPINFOHEADER); // 40 byte
	if (size != fwrite((void*)(&pimage->bmpInfo->bmiHeader), 1, size, pfile)) {
		rcode = ERR_IO_WRITEDATA; goto RET;
	}

	// 1.3. д���ɫ��(ֻ�� 1-bit, 8-bit, 16-bit ����д���ɫ��)
	if (1 == pimage->bmpInfo->bmiHeader.biBitCount || // 1-bit
		8 == pimage->bmpInfo->bmiHeader.biBitCount || // 8-bit
		16== pimage->bmpInfo->bmiHeader.biBitCount) { // 16-bit
		size = pimage->bmpInfo->bmiHeader.biClrUsed*sizeof(RGBQUAD);
		if (size != fwrite((void*)(pimage->bmpInfo->bmiColors), 1, size, pfile)) {
			rcode = ERR_IO_WRITEDATA; goto RET;
		}
	}
	
	// 1.4. д��λͼ����
	if (0 != pimage->bmpInfo->bmiHeader.biSizeImage) {
		size = pimage->bmpInfo->bmiHeader.biSizeImage;
		if (size != fwrite((void*)(pimage->data), 1, size, pfile)) {
			rcode = ERR_IO_WRITEDATA; goto RET;
		}
	}
	
RET:
	return rcode;
}

//===================================================================
static int loadbmpheader(FILE * const pfile, unsigned long * const pdwfilesize, unsigned long * const pdwoffbits, BITMAPINFOHEADER * const pbmih)
{
	int rcode = NO_ERROR;

	size_t size;
	BITMAPFILEHEADER bmfh;
	
	//
	// 0. ��ȫ���
	//
	assert(pfile);
	assert(pbmih);
	assert(pdwoffbits);
	assert(pdwfilesize);
	if (NULL == pfile || NULL == pbmih || NULL == pdwoffbits || NULL == pdwfilesize) {
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. ��ȡ BITMAPINFOHEADER ��������
	//
	// 1.1. ��ȡ BITMAPFILEHEADER ��Ϣ������ļ�����
	size = sizeof(BITMAPFILEHEADER);
	if (size != fread((void*)&bmfh, 1, size, pfile)) {
		rcode = ERR_IO_READDATA; goto RET;
	}
	if (*((WORD*)"BM") != bmfh.bfType) { // bitmap �ļ����ͼ��
		rcode = ERR_IO_FORMAT; goto RET;
	}

	// 1.2. ��ȡ BITMAPINFOHEADER ��Ϣ
	size = sizeof(BITMAPINFOHEADER);
	if (size != fread((void*)pbmih, 1, size, pfile)) {
		rcode = ERR_IO_READDATA; goto RET;
	}

	//
	// 2. �����ļ������ṹ��С���ֽ���Ϣ
	//
	(*pdwfilesize) = bmfh.bfSize;
	(*pdwoffbits)  = bmfh.bfOffBits;

RET:
	return rcode;
}

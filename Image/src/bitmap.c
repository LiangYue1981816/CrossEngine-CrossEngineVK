/////////////////////////////////////////////////////////////////////
// bitmap.c
//
// 功  能: 提供对*.bmp影像的I/O操作，不支持压缩
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//         10:59 2007-01-08
//         20:52 2007-09-17
//===================================================================

#include "_image.h"

static int loadbmpheader(FILE * const pfile, unsigned long * const pdwfilesize, unsigned long * const pdwoffbits, BITMAPINFOHEADER * const pbmih);

//////////////////////////////////////////////////////////////////
//                       位图结构:                              //
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
// 名  称: IMAGE_LoadBmpHeader
//
// 功  能: 加载 .bmp 影像文件 BITMAPINFOHEADER 部分
//
// 参  数: char             * const pszfilename    文件路径
//         BITMAPINFOHEADER * const pbmpinfoheader 头指针
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadBmpHeader(char * const pszfilename, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 安全检查
	//
	assert(pszfilename);
	assert(pbmpinfoheader);
	if (NULL == pszfilename || NULL == pbmpinfoheader) { // 输入参数错误
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. 以二进制只读方式打开文件
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 读取文件信息
	//
	rcode = IMAGE_LoadBmpHeaderEx(pfile, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (NULL != pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadBmpHeaderEx
//
// 功  能: 加载 .bmp 影像文件 BITMAPINFOHEADER 部分
//
// 参  数: FILE             * const pfile          文件指针
//         BITMAPINFOHEADER * const pbmpinfoheader 头指针
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadBmpHeaderEx(FILE * const pfile, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	unsigned long dwfilesize, dwoffbits; // dwfilesize = dwoffbits + IMAGE DATA
                                         // dwoffbits  = BITMAPFILEHEADER + BITMAPINFOHEADER + n*RGBQUAD

	//
	// 0. 安全检查
	//
	assert(pfile);
	assert(pbmpinfoheader);
	if (NULL == pfile || NULL == pbmpinfoheader) { // 输入参数错误
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. 读取文件信息
	//
	rcode = loadbmpheader(pfile, &dwfilesize, &dwoffbits, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadBmp
//
// 功  能: 加载 .bmp 影像文件
//
// 参  数: char  * const pszfilename 文件路径
//         IMAGE * const pimage      影像头指针
//
// 返回值：int
//===================================================================
EXPORT_C int IMAGE_LoadBmp(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 安全检查
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // 输入参数错误
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		rcode = ERR_IMAGE; goto RET;
	};

	//
	// 1. 以二进制只读方式打开文件
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 加载Bitmap文件
	//
	rcode = IMAGE_LoadBmpEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (NULL != pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadBmpEx
//
// 功  能: 加载 .bmp 影像文件
//
// 参  数: FILE  * const pfile  文件指针
//         IMAGE * const pimage 影像头指针
//
// 返回值：int
//===================================================================
EXPORT_C int IMAGE_LoadBmpEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	size_t size;
	BITMAPINFOHEADER bmih;
	unsigned long dwoffset, dwfilesize, dwoffbits; // dwfilesize = dwoffbits + IMAGE DATA
                                                   // dwoffbits  = BITMAPFILEHEADER + BITMAPINFOHEADER + n*RGBQUAD

	//
	// 0. 安全检查
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // 输入参数错误
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		rcode = ERR_IMAGE; goto RET;
	}

	// 获得当前文件偏移量
	dwoffset = ftell(pfile);

	//
	// 1. 读取 bmp 文件头，并进行检察其类型
	//
	rcode = loadbmpheader(pfile, &dwfilesize, &dwoffbits, &bmih);
	if (rcode != NO_ERROR) goto RET;

	if (1 != bmih.biBitCount &&
		8 != bmih.biBitCount &&
		16!= bmih.biBitCount &&
		24!= bmih.biBitCount &&
		32!= bmih.biBitCount) { // 影像文件色深检查
		rcode = ERR_IO_BITCOUNT; goto RET;
	}
	if (0 != bmih.biCompression) { // 影像文件压缩检查
		rcode = ERR_IO_COMPRESS; goto RET;
	}

	//
	// 2. 根据影像文件头信息分配存储
	//    (注意：通过 IMAGE_AllocImage 分配的 8-bit 影像默认是灰阶影像)
	//
	rcode = IMAGE_AllocImage(pimage, bmih.biWidth, bmih.biHeight, bmih.biBitCount);
	if (rcode != NO_ERROR) goto RET;
	
	//
	// 3. 读取调色板信息
	//
	if (1 == bmih.biBitCount || // 1-bit
		8 == bmih.biBitCount) { // 8-bit
		// 4.1. 定位调色板位置
		rcode = fseek(pfile, dwoffset, SEEK_SET);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		rcode = fseek(pfile, sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER), SEEK_CUR);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		// 4.2. 读取调色板信息
		size = dwoffbits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		if (size != fread((void*)(pimage->bmpInfo->bmiColors), 1, size, pfile) ) {
			rcode = ERR_IO_READDATA; goto ERR;
		}
	}

	//
	// 4. 读取数据信息
	//
	size = min(dwfilesize-dwoffbits, pimage->bmpInfo->bmiHeader.biSizeImage);
	if (0 != size) {
		// 4.1. 定位数据信息位置
		rcode = fseek(pfile, dwoffset, SEEK_SET);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		rcode = fseek(pfile, dwoffbits, SEEK_CUR);
		if (rcode != NO_ERROR) { rcode = ERR_IO_SEEK; goto ERR; }

		// 4.2. 读取数据信息
		if (size != fread((void*)(pimage->data), 1, size, pfile) ) {
			rcode = ERR_IO_READDATA; goto ERR;
		}
	}

	//
	// 5. 转换为内部处理的标准影像
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
// 名  称: IMAGE_SaveBmp
//
// 功  能: 保存标准非压缩的 .bmp 影像文件
//
// 参  数: char  * const pszfilename 文件路径
//         IMAGE * const pimage      影像头指针
//
// 返回值：int 
//===================================================================
EXPORT_C int IMAGE_SaveBmp(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 安全检查
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // 输入参数错误
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL == pimage->data) { // 影像数据有效性检查
		rcode = ERR_IMAGE; goto RET;
	}

	//
	// 1. 打开文件
	//
	pfile = fopen(pszfilename, "wb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 读取Bitmp影像文件
	//
	rcode = IMAGE_SaveBmpEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;
	
RET:
	//
	// 3. 关闭文件
	//
	if (NULL != pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_SaveBmpEx
//
// 功  能: 保存标准非压缩的 .bmp 影像文件
//
// 参  数: FILE  * const pfile  文件指针
//         IMAGE * const pimage 影像头指针
//
// 返回值：int 
//===================================================================
EXPORT_C int IMAGE_SaveBmpEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	size_t size;
	BITMAPFILEHEADER bmfh;

	//
	// 0. 安全检查
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // 输入参数错误
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimage || NULL == pimage->data) { // 影像数据有效性检查
		rcode = ERR_IMAGE; goto RET;
	}

	//
	// 1. 将 bitmap 影像数据写入 bmp 影像文件
	//
	// 1.1.1. 设置 BITMAPFILEHEADER 结构信息
	bmfh.bfReserved1 = 0;			// 必须是0
	bmfh.bfReserved2 = 0;			// 必须是0
	bmfh.bfType = *((WORD*)"BM");	// 位图必须是 "BM"
	bmfh.bfOffBits =				// BITMAPFILEHEADER + BITMAPINFOHEADER + 调色板
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pimage->bmpInfo->bmiHeader.biClrUsed*sizeof(RGBQUAD);
	bmfh.bfSize =					// bmfh.bfOffBits + Image Data Size
		bmfh.bfOffBits + pimage->bmpInfo->bmiHeader.biSizeImage;

	// 1.1.2. 写入 BITMAPFILEHEADER
	size = sizeof(BITMAPFILEHEADER); // 14 byte
	if (size != fwrite((void*)(&bmfh), 1, size, pfile)) {
		rcode = ERR_IO_WRITEDATA; goto RET;
	}

	// 1.2. 写入 BITMAPINFOHEADER
	size = sizeof(BITMAPINFOHEADER); // 40 byte
	if (size != fwrite((void*)(&pimage->bmpInfo->bmiHeader), 1, size, pfile)) {
		rcode = ERR_IO_WRITEDATA; goto RET;
	}

	// 1.3. 写入调色板(只有 1-bit, 8-bit, 16-bit 才需写入调色板)
	if (1 == pimage->bmpInfo->bmiHeader.biBitCount || // 1-bit
		8 == pimage->bmpInfo->bmiHeader.biBitCount || // 8-bit
		16== pimage->bmpInfo->bmiHeader.biBitCount) { // 16-bit
		size = pimage->bmpInfo->bmiHeader.biClrUsed*sizeof(RGBQUAD);
		if (size != fwrite((void*)(pimage->bmpInfo->bmiColors), 1, size, pfile)) {
			rcode = ERR_IO_WRITEDATA; goto RET;
		}
	}
	
	// 1.4. 写入位图数据
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
	// 0. 安全检查
	//
	assert(pfile);
	assert(pbmih);
	assert(pdwoffbits);
	assert(pdwfilesize);
	if (NULL == pfile || NULL == pbmih || NULL == pdwoffbits || NULL == pdwfilesize) {
		rcode = ERR_PARAMETER; goto RET;
	}

	//
	// 1. 读取 BITMAPINFOHEADER 部分数据
	//
	// 1.1. 读取 BITMAPFILEHEADER 信息，检查文件类型
	size = sizeof(BITMAPFILEHEADER);
	if (size != fread((void*)&bmfh, 1, size, pfile)) {
		rcode = ERR_IO_READDATA; goto RET;
	}
	if (*((WORD*)"BM") != bmfh.bfType) { // bitmap 文件类型检查
		rcode = ERR_IO_FORMAT; goto RET;
	}

	// 1.2. 读取 BITMAPINFOHEADER 信息
	size = sizeof(BITMAPINFOHEADER);
	if (size != fread((void*)pbmih, 1, size, pfile)) {
		rcode = ERR_IO_READDATA; goto RET;
	}

	//
	// 2. 保存文件各个结构大小的字节信息
	//
	(*pdwfilesize) = bmfh.bfSize;
	(*pdwoffbits)  = bmfh.bfOffBits;

RET:
	return rcode;
}

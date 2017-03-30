/////////////////////////////////////////////////////////////////////
// jpeg.c
//
// 功  能: 提供对*.jpg影像的I/O操作
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//         10:27 2006-03-15
//         13:22 2006-03-24
//         09:45 2006-03-25
//         10:53 2006-03-31
//         21:11 2007-09-17
//===================================================================

#include "_image.h"
#include "jerror.h"
#include "jpeglib.h"
#include "jconfig.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadJpgHeader
//
// 功  能: 获得jpeg文件BITMAP头信息
//
// 参  数: char             * const pszfilename    文件全路经
//         BITMAPINFOHEADER * const pbmpinfoheader BITMAP信息头
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadJpgHeader(char * const pszfilename, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 参数安全检查
	//
	assert(pszfilename);
	assert(pbmpinfoheader);
	if (NULL == pszfilename || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. 打开文件
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 读取文件头
	//
	rcode = IMAGE_LoadJpgHeaderEx(pfile, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadJpgHeaderEx
//
// 功  能: 获得jpeg文件BITMAP头信息
//
// 参  数: FILE             * const pfile          文件指针
//         BITMAPINFOHEADER * const pbmpinfoheader BITMAP信息头
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadJpgHeaderEx(FILE * const pfile, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	unsigned char mark[2];

	struct jpeg_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;

	//
	// 0. 参数安全检查
	//
	assert(pfile);
	assert(pbmpinfoheader);
	if (NULL == pfile || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. 创建压缩
	//
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pfile);

	//
	// 2. 读取文件头，并进行格式检查
	//
	fread(mark, 2, sizeof(unsigned char), pfile); fseek(pfile, -2, SEEK_CUR);
	if (0xFF != mark[0] || 0xD8 != mark[1]) { rcode = ERR_IO_FORMAT; goto RET; }

	jpeg_read_header(&cinfo, TRUE);
	if ((1 != cinfo.num_components && 3 != cinfo.num_components) || // 影像色深检查
		 0 == cinfo.image_width || 0 == cinfo.image_height ||		// 影像尺寸检查
		 JCS_UNKNOWN == cinfo.jpeg_color_space) {					// 影像调色板检查
		rcode = ERR_IO_FORMAT; goto RET;
	}

	//
	// 3. 填充BITMAPINFOHEADER结构
	//
	pbmpinfoheader->biWidth = cinfo.image_width;
	pbmpinfoheader->biHeight = cinfo.image_height;
	pbmpinfoheader->biBitCount = cinfo.num_components*8;

RET:
	return rcode;
}

////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadJpg
//
// 功  能: 加载jpeg文件
//
// 参  数: char  * const pszfilename 文件全路经
//         IMAGE * const pimage      影像文件结构
//
// 返回值: int
//==============================================================
EXPORT_C int IMAGE_LoadJpg(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 参数安全检查
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // 输入参数错误
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}

	//
	// 1. 打开文件
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 加载Jpeg文件
	//
	rcode = IMAGE_LoadJpgEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (pfile) fclose(pfile);

	return rcode;
}

////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadJpgEx
//
// 功  能: 加载jpeg文件
//
// 参  数: FILE  * const pfile  文件指针
//         IMAGE * const pimage 影像文件结构
//
// 返回值: int
//==============================================================
EXPORT_C int IMAGE_LoadJpgEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	unsigned long i;
	unsigned char temp;
	unsigned char *pdata = NULL;
	unsigned char mark[2];

	struct jpeg_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;

	//
	// 0. 参数安全检查
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // 输入参数错误
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}

	//
	// 1. 创建压缩
	//
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pfile);

	//
	// 2. 读取文件头，并进行格式检查
	//
	fread(mark, 2, sizeof(unsigned char), pfile); fseek(pfile, -2, SEEK_CUR);
	if (0xFF != mark[0] || 0xD8 != mark[1]) { rcode = ERR_IO_FORMAT; goto RET; }

	jpeg_read_header(&cinfo, TRUE);
	if ((1 != cinfo.num_components && 3 != cinfo.num_components) || // 影像色深检查
		 0 == cinfo.image_width || 0 == cinfo.image_height ||		// 影像尺寸检查
		 JCS_UNKNOWN == cinfo.jpeg_color_space) {					// 影像调色板检查
		rcode = ERR_IO_FORMAT; goto RET;
	}

	//
	// 3. 根据影像信息分配影像
	//
	rcode = IMAGE_AllocImage(pimage, cinfo.image_width, cinfo.image_height, cinfo.num_components*8);
	if (rcode != NO_ERROR) goto RET;

	//
	// 4. 开始解压缩
	//
	jpeg_start_decompress(&cinfo);
	pdata = pimage->data + (IMAGE_HEIGHT(pimage)-1)*pimage->buffWidth;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &pdata, 1);
		// 注意：对于24bit影像，压缩使用RGB,RGB,RGB...形
		//		 式，而存储状态是BGR,BGR,BGR...需要进行R,B
		//		 交换操作才能得到正确结果
		if (24 == IMAGE_BITCOUNT(pimage)) {
			for (i = 0; i < pimage->buffWidth - 3; i += 3) {
				temp = pdata[i+2]; pdata[i+2] = pdata[i]; pdata[i] = temp;
			}
		}
		pdata -= pimage->buffWidth;
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//
	// 5. 将非标准影像转换到标准影像
	//
	rcode = _converttostdimage(pimage);
	if (rcode != NO_ERROR) IMAGE_FreeImage(pimage);

RET:
	return rcode;
}

////////////////////////////////////////////////////////////////
// 名  称: IMAGE_SaveJpg
//
// 功  能: 保存jpeg文件
//
// 参  数: char  * const pszfilename 文件全路经
//         IMAGE * const pimage      影像文件结构
//         int     const nquality    保存质量
//
// 返回值: int
//==============================================================
EXPORT_C int IMAGE_SaveJpg(char * const pszfilename, IMAGE * const pimage, int const nquality)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 参数检查，只支持8bit和24bit影像
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename || nquality < 0 || nquality > 100) { // 输入参数无效
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL == pimage->data) { // 影像无效
		return ERR_IMAGE;
	}
	if (1 == IMAGE_BITCOUNT(pimage)) { // 色深无效
		return ERR_BITCOUNT;
	}

	//
	// 1. 以二进制方式打开文件
	//
	pfile = fopen(pszfilename, "wb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 保存Jpeg文件
	//
	rcode = IMAGE_SaveJpgEx(pfile, pimage, nquality);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (pfile) fclose(pfile);

	return rcode;
}

////////////////////////////////////////////////////////////////
// 名  称: IMAGE_SaveJpgEx
//
// 功  能: 保存jpeg文件
//
// 参  数: FILE  * const pfile    文件指针
//         IMAGE * const pimage   影像文件结构
//         int     const nquality 保存质量
//
// 返回值: int
//==============================================================
EXPORT_C int IMAGE_SaveJpgEx(FILE * const pfile, IMAGE * const pimage, int const nquality)
{
	unsigned long i;
	unsigned char temp;
	unsigned char *pdata = NULL;

	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;

	//
	// 0. 参数检查，只支持8bit和24bit影像
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile || nquality < 0 || nquality > 100) { // 输入参数无效
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL == pimage->data) { // 影像无效
		return ERR_IMAGE;
	}
	if (1 == IMAGE_BITCOUNT(pimage)) { // 色深无效
		return ERR_BITCOUNT;
	}

	//
	// 1. 创建压缩
	//
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, pfile);

	//
	// 2. 填写压缩影像的信息
	//
	cinfo.image_width		= IMAGE_WIDTH(pimage);
	cinfo.image_height		= IMAGE_HEIGHT(pimage);
	cinfo.input_components	= IMAGE_BITCOUNT(pimage)/8; // 1对应8bit，3对应24bit
	cinfo.in_color_space	= IMAGE_BITCOUNT(pimage)==24 ? JCS_RGB:JCS_GRAYSCALE;
	
	//
	// 3. 设置压缩参数以及压缩质量(0~100)
	//
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, nquality, TRUE);

	//
	// 4. 进行压缩
	//
	jpeg_start_compress(&cinfo, TRUE);
	pdata = pimage->data + (IMAGE_HEIGHT(pimage)-1)*pimage->buffWidth;
	while (cinfo.next_scanline < cinfo.image_height) {
		// 注意：对于24bit影像，压缩使用RGB,RGB,RGB...形
		//		 式，而存储状态是BGR,BGR,BGR...需要进行R,B
		//		 交换操作才能得到正确结果
		if (24 == IMAGE_BITCOUNT(pimage)) {
			for (i = 0; i < pimage->buffWidth - 3; i += 3) {
				temp = pdata[i+2]; pdata[i+2] = pdata[i]; pdata[i] = temp;
			}
		}
		jpeg_write_scanlines(&cinfo, &pdata, TRUE);
		pdata -= pimage->buffWidth;
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	return NO_ERROR;
}

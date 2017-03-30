/////////////////////////////////////////////////////////////////////
// ercode.c
//
// 功  能: 侵蚀亮化
//
// 修  订: 10:00 2005-01-02
//         12:23 2005-06-19
//         17:34 2007-01-09
//         09:14 2015-01-15
//===================================================================

#include "_image.h"

static void * erode8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int amount);
static void * erode24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int amount);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Erode
//
// 功  能: 侵蚀亮化
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         int     const namount 亮化值
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Erode(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const namount)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (namount < 0 || namount > 255) { // 要求参数在0~255之间
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // 要求影像处理区域不得小于 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1 根据不同色深分别调用不同函数
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: erode8bit(pimgsrc, pimgdst, &imgwrk, namount); break;
	case 24: erode24bit(pimgsrc, pimgdst, &imgwrk, namount); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * erode8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int amount)
{
	int x, y;
	int width, height;
	
	int pixel;

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
			pixel =
				(ppixel_src_top[x-1] + ppixel_src_top[x] + ppixel_src_top[x+1] +
				 ppixel_src_ths[x-1] + ppixel_src_ths[x] + ppixel_src_ths[x+1] +
				 ppixel_src_btm[x-1] + ppixel_src_btm[x] + ppixel_src_btm[x+1])/9;

			if (pixel > 255 - amount) {
				ppixel_dst_ths[x] = 0x00;
				if (ppixel_dst_ths[x] < ppixel_src_top[x-1]) ppixel_dst_ths[x] = ppixel_src_top[x-1];
				if (ppixel_dst_ths[x] < ppixel_src_top[x  ]) ppixel_dst_ths[x] = ppixel_src_top[x  ];
				if (ppixel_dst_ths[x] < ppixel_src_top[x+1]) ppixel_dst_ths[x] = ppixel_src_top[x+1];
				if (ppixel_dst_ths[x] < ppixel_src_ths[x-1]) ppixel_dst_ths[x] = ppixel_src_ths[x-1];
				if (ppixel_dst_ths[x] < ppixel_src_ths[x  ]) ppixel_dst_ths[x] = ppixel_src_ths[x  ];
				if (ppixel_dst_ths[x] < ppixel_src_ths[x+1]) ppixel_dst_ths[x] = ppixel_src_ths[x+1];
				if (ppixel_dst_ths[x] < ppixel_src_btm[x-1]) ppixel_dst_ths[x] = ppixel_src_btm[x-1];
				if (ppixel_dst_ths[x] < ppixel_src_btm[x  ]) ppixel_dst_ths[x] = ppixel_src_btm[x  ];
				if (ppixel_dst_ths[x] < ppixel_src_btm[x+1]) ppixel_dst_ths[x] = ppixel_src_btm[x+1];
			}
		}
	}

	return NULL;
}

static void * erode24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int amount)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	int pixel;
	int brightest, index;

	IMGGRAY *ppixel_gry_top = NULL;
	IMGGRAY *ppixel_gry_ths = NULL;
	IMGGRAY *ppixel_gry_btm = NULL;

	IMGRGB *ppixel_src_top = NULL;
	IMGRGB *ppixel_src_ths = NULL;
	IMGRGB *ppixel_src_btm = NULL;
	IMGRGB *ppixel_dst_ths = NULL;

	IMAGE imggry;
	IMAGE_ZeroImage(&imggry);

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	rcode = IMAGE_AllocImage(&imggry, IMAGE_WIDTH(pimgwrk), IMAGE_HEIGHT(pimgwrk), 8);
	if (rcode != NO_ERROR) goto RET;

	rcode = IMAGE_ColorToGray(pimgwrk, &imggry, GRAY_STD);
	if (rcode != NO_ERROR) goto RET;

	imggry.sx = pimgwrk->sx; imggry.sy = pimgwrk->sy;
	imggry.ex = pimgwrk->ex; imggry.ey = pimgwrk->ey;

	for (y = 0; y < height; y++) {
		ppixel_gry_top = (IMGGRAY *)(&(imggry.matrix[imggry.sy+y-1][imggry.sx]));
		ppixel_gry_ths = (IMGGRAY *)(&(imggry.matrix[imggry.sy+y  ][imggry.sx]));
		ppixel_gry_btm = (IMGGRAY *)(&(imggry.matrix[imggry.sy+y+1][imggry.sx]));

		ppixel_src_top = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx*3]));
		ppixel_src_ths = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx*3]));
		ppixel_src_btm = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx*3]));
		ppixel_dst_ths = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			pixel =
				(ppixel_gry_top[x-1] + ppixel_gry_top[x] + ppixel_gry_top[x+1] +
				 ppixel_gry_ths[x-1] + ppixel_gry_ths[x] + ppixel_gry_ths[x+1] +
				 ppixel_gry_btm[x-1] + ppixel_gry_btm[x] + ppixel_gry_btm[x+1])/9;

			if (pixel < 255 - amount) {
				//	00 01 02
				//	10 11 12
				//	20 21 22
				index = -1;
				brightest = 0x00;
				if (brightest < ppixel_gry_top[x-1]) { brightest = ppixel_gry_top[x-1]; index = 00; }
				if (brightest < ppixel_gry_top[x  ]) { brightest = ppixel_gry_top[x  ]; index = 01; }
				if (brightest < ppixel_gry_top[x+1]) { brightest = ppixel_gry_top[x+1]; index = 02; }
				if (brightest < ppixel_gry_ths[x-1]) { brightest = ppixel_gry_ths[x-1]; index = 10; }
				if (brightest < ppixel_gry_ths[x  ]) { brightest = ppixel_gry_ths[x  ]; index = 11; }
				if (brightest < ppixel_gry_ths[x+1]) { brightest = ppixel_gry_ths[x+1]; index = 12; }
				if (brightest < ppixel_gry_btm[x-1]) { brightest = ppixel_gry_btm[x-1]; index = 20; }
				if (brightest < ppixel_gry_btm[x  ]) { brightest = ppixel_gry_btm[x  ]; index = 21; }
				if (brightest < ppixel_gry_btm[x+1]) { brightest = ppixel_gry_btm[x+1]; index = 22; }
				if (-1 == index) continue;

				switch (index) {
					// last line
				case 00:
					ppixel_dst_ths[x].R = ppixel_src_top[x-1].R;
					ppixel_dst_ths[x].G = ppixel_src_top[x-1].G;
					ppixel_dst_ths[x].B = ppixel_src_top[x-1].B;
					break;
				case 01:
					ppixel_dst_ths[x].R = ppixel_src_top[x  ].R;
					ppixel_dst_ths[x].G = ppixel_src_top[x  ].G;
					ppixel_dst_ths[x].B = ppixel_src_top[x  ].B;
					break;
				case 02:
					ppixel_dst_ths[x].R = ppixel_src_top[x+1].R;
					ppixel_dst_ths[x].G = ppixel_src_top[x+1].G;
					ppixel_dst_ths[x].B = ppixel_src_top[x+1].B;
					break;
					// this line
				case 10:
					ppixel_dst_ths[x].R = ppixel_src_ths[x-1].R;
					ppixel_dst_ths[x].G = ppixel_src_ths[x-1].G;
					ppixel_dst_ths[x].B = ppixel_src_ths[x-1].B;
					break;
				case 11:
					ppixel_dst_ths[x].R = ppixel_src_ths[x  ].R;
					ppixel_dst_ths[x].G = ppixel_src_ths[x  ].G;
					ppixel_dst_ths[x].B = ppixel_src_ths[x  ].B;
					break;
				case 12:
					ppixel_dst_ths[x].R = ppixel_src_ths[x+1].R;
					ppixel_dst_ths[x].G = ppixel_src_ths[x+1].G;
					ppixel_dst_ths[x].B = ppixel_src_ths[x+1].B;
					break;
					// next line
				case 20:
					ppixel_dst_ths[x].R = ppixel_src_btm[x-1].R;
					ppixel_dst_ths[x].G = ppixel_src_btm[x-1].G;
					ppixel_dst_ths[x].B = ppixel_src_btm[x-1].B;
					break;
				case 21:
					ppixel_dst_ths[x].R = ppixel_src_btm[x  ].R;
					ppixel_dst_ths[x].G = ppixel_src_btm[x  ].G;
					ppixel_dst_ths[x].B = ppixel_src_btm[x  ].B;
					break;
				case 22:
					ppixel_dst_ths[x].R = ppixel_src_btm[x+1].R;
					ppixel_dst_ths[x].G = ppixel_src_btm[x+1].G;
					ppixel_dst_ths[x].B = ppixel_src_btm[x+1].B;
					break;
				}
			}
		}
	}

RET:
	IMAGE_FreeImage(&imggry);
	return NULL;
}

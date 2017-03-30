#include "_image.h"
#include "_wav_alloc.h"
#include "_wav_gen.h"
#include "_wav_filters_extern.h"

/////////////////////////////////////////////////////////////////////
// 名  称: extend_periodic
//
// 功  能: 周期前后扩展数据边缘
//
// 参  数: float * const data 数据缓冲
//         int     const N    数据元素个数
//         int     const ofs  扩展范围
//
// 返回值: void
//
// Remark: Extend data by ofs pixels on both ends using periodic 
//         symmetry. data should be "pointing in" to allow up to data[-ofs]
//         and data[N-1+ofs]. 
//         ofs>=N is handled.
//
// eg:     data: 1 2 3 4 5 6 周其扩展2个元素 => 5 6 1 2 3 4 5 6 1 2
//===================================================================
static void extend_periodic(float * const data, int const N, int const ofs)
{
	int i;
	
	for (i = 1; i <= ofs; i++) {
		data[-i] = data[N-i]; // start
		data[N-1+i] = data[i-1]; // end
	}
}

/////////////////////////////////////////////////////////////////////
// 名  称: extend_mirror
//
// 功  能: 镜像扩展数据边缘
//
// 参  数: float * const data  数据缓冲
//         int     const N     数据元素个数
//         int     const ofs   扩展范围
//         int     const phase 镜像方式
//
// 返回值: void
//
// Remark: Extend data by ofs pixels on both ends using mirror 
//         symmetry. data should be "pointing in" to allow up to data[-ofs]
//         and data[N-1+ofs]. 
//
//         There are two possible "mirrors", i.e., data[-1]=data[1] (mirror on 0) 
//         or data[-1]=data[0] (mirror "between" -1 and 0). phase is either 0, 1, or 2
//         to select among these cases. See if(phase==... below.
//
// eg:     data: 1 2 3 4 5 6 对称扩展2个像素
//         phase = 0: => 3 2 1 2 3 4 5 6 6 5
//         phase = 1: => 2 1 1 2 3 4 5 6 5 4
//         phsdr = 2: => 3 2 1 2 3 4 5 6 5 4
//===================================================================
static void extend_mirror(float * const data, int const N, int const ofs, int const phase)
{
	int i;
	int phase1, phase2;

	// parameter check.
	if ((phase < 0) || (phase > 2))
		return;

	if (2 == phase){
		// At left boundary the mirror is on 0.
		phase1 = 0;
		// At right boundary the mirror is on N-1.
		phase2 = 1;
	}
	else {
		// phase == 0, at left mirror on 0, and at right it's between.
		// phase == 1, at left mirror between, and at right it's on N-1.
		phase1 = phase2 = phase;
	}

	if (1 == N) {
		// Only one data.
		for (i = 1; i <= ofs; i++) {
			data[-i] = data[0]; // start
			data[N-1+i] = data[0]; // end
		}
	}
	else {
		// Otherwise.
		for (i = 1; i <= ofs; i++) {
			data[-i] = data[i-phase1]; // start
			data[N-1+i] = data[N-phase2-i]; // end
		}
	}
}

/////////////////////////////////////////////////////////////////////
// 名  称: filter_n_decimate
//
// 功  能: 1维小波正向变换
//
// 参  数: float * const data   数据缓冲
//         float * const coef   输出缓冲
//         int     const N      数据元素个数
//         float * const filter 离散滤波器数据缓冲（通过选择不同的滤波器达到不同的效果）
//         int     const Nf     离散滤波器中数据个数
//         int     const beg    开始位置
//         int     const ofs    抽样频率
//
// 返回值: int
//
// Remark: This routine is used in evaluating forward wavelet transforms.
//
//         data of dimension N contains the input.
//         float *filter contains the Nf filter taps.
//         ofs is the decimation factor.
//
//         filtering starts at beg and continues until N+beg, yielding N/ofs samples. 
//         data needs to be padded suitably at both ends.
//         This hoopla about the starting point is in place to ensure 
//         correct boundary processing when evaluating wavelet transforms.
//
//         coef returns the result of the filtering and decimation.
// 
//         Viewed as a transform the basis function that generates the
//         coefficient of index 0 is "<- fliplr(filter) -> (beg)", 
//         i.e., the scalar product obtained by positioning 
//         the flipped filter (formed into a row vector) 
//         with the right side positioned on top of sample beg in data.
//===================================================================
static int filter_n_decimate(float * const data, float * const coef, int const N, float * const filter, int const Nf, int const beg, int const ofs)
{
	int i, j, k;
	float temp;
	
	for (k = 0, i = beg; i < N+beg; i += ofs, k++) {
		for(temp = 0, j = i; j > i-Nf; j--)
			temp += data[j]*filter[i-j];
			
		coef[k] = temp;
	}

	return k;
}

/////////////////////////////////////////////////////////////////////
// 名  称: upsample_n_filter
//
// 功  能: 1维小波逆向变换
//
// 参  数: float * const coef   插值频率缓冲
//         float * const data   输出数据缓冲
//         int     const N      数据元素个数
//         float * const filter 离散滤波器数据缓冲
//         int     const Nf     离散滤波器中数据个数
//         int     const beg    开始位置
//         int     const ofs    抽样频率
//
// 返回值: void
//
// Remark: This routine is used in evaluating inverse wavelet transforms.
//
//         Upsample and filter to implement a synthesis filter bank.
//         Basically the inverse of filter_n_decimate() above.
//         No actual upsampling etc., to avoid zero multiplies.
//
//         coef is the input and contains coefficients resulting from 
//         filtering and decimation by ofs. 
//
//         Because this routine is typically called twice for inverse wavelet
//         transforms, once for the low band and once for high, data is incremented via +=.
//         Hence data should initially be set to 0.
//
//         Viewed as a transform, filter_n_decimate above starts by putting the 
//         "flipped" forward filter at begf, i.e.,  <- fliplr(ffilter) ->(begf)
//         The resulting coefficient is at index 0.
//         Here we are upsampling by ofs and filtering with the inv. filter
//         time advanced by fbeg so the coefficient at index 0, multiplies the basis function
//         (-fbeg)<-(ifilter)->(Nf-1-fbeg=beg).
// 
//         How is that for confusing? All of this makes sense,
//         when you write it down carefully. But who has the time ...:)
//===================================================================
static void upsample_n_filter(float * const coef, float * const data, int const N, float * const filter, int const Nf, int const beg, int const ofs)
{
	int i, j, l, n, p, fbeg;
	float temp;

	fbeg = Nf - beg - 1;
	for (i = 0; i < N; i++) {
		n = (i+fbeg)%ofs;
		p = (i+fbeg)/ofs;
		
		for (temp = 0, l = 0, j = n; j < Nf; j += ofs, l++)
			temp += coef[p-l]*filter[j];

		// += for successive calls for low and high bands. 
		data[i] += temp;
	}
}

/////////////////////////////////////////////////////////////////////
// 名  称: filt_n_dec_all_rows
//
// 功  能: 2维的小波行正向变换
//
// 参  数: float ** const image 影像数据
//         int      const Ni    影像高
//         int      const Nj    影像宽
//         float  * const lp    低频滤波器数据缓冲
//         int      const Nl    低频滤波器数据元素个数
//         float  * const hp    高频滤波器数据缓冲
//         int      const Nh    高频滤波器数据元素个数
//
// 返回值: void
//
// Remark: Used in forward wavelet trf.
//
//         All rows of the image are run through the dual filter bank
//         given by lp and hp. Results are returned in image "in place".
//         low freq. coefficients start at 0 and end at N/2, where high freq.
//         info starts.
//===================================================================
void filt_n_dec_all_rows(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh)
{
	int i, j, ext, ofs;
	float *data = NULL;

	ext = max(Nl, Nh);
	data = allocate_1d_float((Nj+2*ext)); // Extend width = image width + 2*max filter width
	// Point in for required extensions.
	data += ext;

	// offset for the location where high band coefficients
	// should be copied.
	ofs = Nj>>1; // half width of image

	for (i = 0; i < Ni; i++) {		
		// Copy row.
		for (j = 0; j < Nj; j++)
			data[j] = image[i][j];

		// Extend.
		if(PS)
		    // Periodic at end points
			extend_periodic(data, Nj, ext);
		else
			// Mirrors at end points.
			extend_mirror(data, Nj, ext, 2);

		// forward low pass, save to left half image.
		filter_n_decimate(data, image[i], Nj, lp, Nl, begflp, 2);
		// forward high pass, save to right haft image.
		filter_n_decimate(data, image[i]+ofs, Nj, hp, Nh, begfhp, 2);
	}

	data -= ext;
	free((void *)data);
}

/////////////////////////////////////////////////////////////////////
// 名  称: filt_n_dec_all_cols
//
// 功  能: 2维的小波列正向变换
//
// 参  数: float ** const image 影像数据
//         int      const Ni    影像高
//         int      const Nj    影像宽
//         float  * const lp    低频滤波器数据缓冲
//         int      const Nl    低频滤波器数据元素个数
//         float  * const hp    高频滤波器数据缓冲
//         int      const Nh    高频滤波器数据元素个数
//
// 返回值: void
//
// Remark: Used in forward wavelet trf.
//
//         Index swapped version of filt_n_dec_all_rows.
//===================================================================
void filt_n_dec_all_cols(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh)
{
	int i, j, ext, ofs;
	float *data1 = NULL, *data2 = NULL;

	ext = max(Nl, Nh);
	data1 = allocate_1d_float((Ni+2*ext)); // Extend height = image height + 2*max filter height.
	// Point in for required extensions.
	data1 += ext;

	// Need second array since cannot access columns directly via pointers.
	data2 = allocate_1d_float(Ni);

	// offset for the location where high band coefficients
	// should be copied.
	ofs = Ni>>1; // half height of image.

	for (j = 0; j < Nj; j++) {		
		// Copy column.
		for(i = 0; i < Ni; i++)
			data1[i] = image[i][j];

		// Extend.
		if (PS)
			// Periodic at end points
			extend_periodic(data1,Ni,ext);
		else
			// Mirrors at end points.
			extend_mirror(data1,Ni,ext,2);
		
		// forward low pass, save to top half image.
		filter_n_decimate(data1, data2, Ni, lp, Nl, begflp, 2);
		// forward high pass, save to bottom half image.
		filter_n_decimate(data1, data2+ofs, Ni, hp, Nh, begfhp, 2);
		
		// Save data to image.
		for (i = 0; i < Ni; i++)
			image[i][j] = data2[i];
	}

	data1 -= ext;
	free((void *)data1);
	free((void *)data2);
}

/////////////////////////////////////////////////////////////////////
// 名  称: ups_n_filt_all_rows
//
// 功  能: 2维的小波行反向变换
//
// 参  数: float ** const image 影像数据
//         int      const Ni    影像高
//         int      const Nj    影像宽
//         float  * const lp    低频滤波器数据缓冲
//         int      const Nl    低频滤波器数据元素个数
//         float  * const hp    高频滤波器数据缓冲
//         int      const Nh    高频滤波器数据元素个数
//         int      const lev
//         int    * const shift_arr
//
// 返回值: void
//
// Remark: Used in inverse wavelet trf.
//
//         All rows of the image are run through the dual synthesis filterbank
//         given by lp and hp having number of taps Nl and Nh respectively. 
//         Results are returned in image "in place".
//
//         Here we actaully need to know the applied shift to the transform,
//         hence the passed parameters lev and shift_arr.
//===================================================================
void ups_n_filt_all_rows(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh, int const lev, int * const shift_arr)
{
	int i, j, k, ext, ofs;
	float *data1 = NULL, *data2 = NULL;

	ext = max(Nl, Nh); // Extend size
	ofs = Nj>>1;
	
	// Alloc memory.
	data1 = allocate_1d_float((ofs+2*ext));
	data2 = allocate_1d_float((ofs+2*ext));
	data1 += ext;
	data2 += ext;

	for (i = 0; i < Ni; i++) { // Image Height
		for (j = 0; j < ofs; j++) { // Half Image Width
			k = j + ofs;
			// low pass and high pass coefficients.
			data1[j] = image[i][j]; image[i][j] = 0; // Left Image Data
			data2[j] = image[i][k]; image[i][k] = 0; // Right Image Data
		}

		// Take care of the extension at the boundaries.
		if(PS) {
			// periodic at end points
			extend_periodic(data1, ofs, ext);
			extend_periodic(data2, ofs, ext);
		}
		else {
			// shift_arr[lev]=0 OR 1.
			// The symmetric banks used are positioned so that on the left side
			// the forward lowpass filter has its point of symmetry exactly on top of 0
			// and forward high pass filter has its point of symmetry exactly on top of 1. 
			// This is coordinated by calling filter_n_decimate() with the proper value of beg.
			//
			// The above is reversed at the right side assuming decimation by 2. 
			// Hence shift_arr[lev]==0 => at left boundary the mirror is at 0, 
			// at right boundary it's between, see extend_mirror().

			// If shift_arr[lev]==1 then the positions for the low and high are reversed
			// since the filters are shifted so if
			// shift_arr[lev]==1 => at left boundary the mirror is between, 
			// at right boundary it's at 0.
			extend_mirror(data1, ofs, ext, shift_arr[lev]);	
			extend_mirror(data2, ofs, ext, 1-shift_arr[lev]);	
		}

		// invert low pass.
		upsample_n_filter(data1, image[i], Nj, lp, Nl, begilp, 2);
		// invert high pass.
		upsample_n_filter(data2, image[i], Nj, hp, Nh, begihp, 2);
	}

	data1 -= ext;
	data2 -= ext;
	free((void *)data1);
	free((void *)data2);
}

/////////////////////////////////////////////////////////////////////
// 名  称: ups_n_filt_all_rows
//
// 功  能: 2维的小波行反向变换
//
// 参  数: float ** const image 影像数据
//         int      const Ni    影像高
//         int      const Nj    影像宽
//         float  * const lp    低频滤波器数据缓冲
//         int      const Nl    低频滤波器数据元素个数
//         float  * const hp    高频滤波器数据缓冲
//         int      const Nh    高频滤波器数据元素个数
//         int      const lev
//         int    * const shift_arr
//
// 返回值: void
//
// Remark: Used in inverse wavelet trf.
//
//         Index swapped version of ups_n_filt_all_rows.
//===================================================================
void ups_n_filt_all_cols(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh, int const lev, int * const shift_arr)
{
	int i, j, k, ext, ofs;
	float *data1 = NULL, *data2 = NULL, *data3 = NULL;

	ext = max(Nl, Nh); // Extend size.
	ofs = Ni>>1;
	
	// Alloc memory.
	data1 = allocate_1d_float((ofs+2*ext));
	data2 = allocate_1d_float((ofs+2*ext));
	data1 += ext;
	data2 += ext;

	// Need third array since cannot access columns directly via pointers.
	data3 = allocate_1d_float(Ni);

	for (j = 0; j < Nj; j++) { // Width
		for (i = 0; i < ofs; i++) { // Half height
			k = i + ofs;
			// low pass and high pass coefficients.
			data1[i] = image[i][j]; // Top image data
			data2[i] = image[k][j]; // Bottom image data
			data3[i] = data3[k] = 0;
		}

		if(PS) {
			// Periodic at end points.
			extend_periodic(data1, ofs, ext);
			extend_periodic(data2, ofs, ext);
		}
		else {
			// Mirror at end points.
			extend_mirror(data1, ofs, ext, shift_arr[lev]);	
			extend_mirror(data2, ofs, ext, 1-shift_arr[lev]);	
		}
		
		upsample_n_filter(data1, data3, Ni, lp, Nl, begilp, 2);
		upsample_n_filter(data2, data3, Ni, hp, Nh, begihp, 2);
		
		// Save data to image
		for(i=0;i<Ni;i++)
			image[i][j] = data3[i];
	}

	data1 -= ext;
	data2 -= ext;
	free((void *)data1);
	free((void *)data2);
	free((void *)data3);
}

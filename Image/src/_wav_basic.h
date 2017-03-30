#ifndef _WAV_BASIC_H_
#define _WAV_BASIC_H_

void filt_n_dec_all_rows(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh);
void filt_n_dec_all_cols(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh);
void ups_n_filt_all_rows(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh, int const lev, int * const shift_arr);
void ups_n_filt_all_cols(float ** const image, int const Ni, int const Nj, float * const lp, int const Nl, float * const hp, int const Nh, int const lev, int * const shift_arr);

#endif

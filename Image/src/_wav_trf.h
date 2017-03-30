#ifndef _WAV_TRF_H_
#define _WAV_TRF_H_

void choose_filter(char const name, int const tap);

void wav2d_inpl(float ** const image, int const Ni, int const Nj, int const levs, float * const lp, int const Nl, float * const hp, int const Nh, char const forw, int * const shift_arr_row, int * const shift_arr_col);
void wavpack2d_inpl(float ** const image, int const Ni, int const Nj, int const levs, float * const lp, int const Nl, float * const hp,int Nh,char forw, int * const shift_arr_row, int * const shift_arr_col);

void complex_wav_forw(float ** const im, float *** const trf, int const Ni, int const Nj, int const levs);
float ** complex_wav_inv(float *** const trf, int const Ni, int const Nj, int const levs);

void complex_wav_pack_forw(float ** const im, float *** const trf, int const Ni, int const Nj, int const levs);
float ** complex_wav_pack_inv(float *** const trf, int const Ni, int const Nj, int const levs);

#endif

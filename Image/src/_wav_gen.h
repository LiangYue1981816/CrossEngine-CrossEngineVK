#ifndef _WAV_GEN_H_
#define _WAV_GEN_H_

// Grow only the LL band after this level 
// (relevant for packet decompositions).
#define LL_ONLY_AFTER_LEV 6

// Maximum level possible in all decompositions.
// Mainly because we are using static allocation in some
// places.
#define MAX_LEV 6
#define MAX_LEV_POW 64 /* 2^MAX_LEV */

#define MAX_ARR_SIZE (MAX_LEV_POW)

#endif

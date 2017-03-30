/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once


typedef signed char                    int8_t;   // 1BYTE
typedef short                          int16_t;  // 2BYTE
typedef int                            int32_t;  // 4BYTE
typedef long long                      int64_t;  // 8BYTE
typedef unsigned char                  uint8_t;  // 1BYTE
typedef unsigned short                 uint16_t; // 2BYTE
typedef unsigned int                   uint32_t; // 4BYTE
typedef unsigned long long             uint64_t; // 8BYTE

#ifdef _PLATFORM_WINDOWS_
typedef unsigned char                  BYTE;     // 1BYTE
typedef unsigned short                 WORD;     // 2BYTE
typedef unsigned long                  DWORD;    // 4BYTE
typedef unsigned long                  COLORREF; // 4BYTE
#else
typedef unsigned char                  BYTE;     // 1BYTE
typedef unsigned short                 WORD;     // 2BYTE
typedef unsigned int                   DWORD;    // 4BYTE
typedef unsigned int                   COLORREF; // 4BYTE
#endif

#ifdef __LP64__
#define POINTER_SIZE                   8         // 8BYTE
#else
#define POINTER_SIZE                   4         // 4BYTE
#endif


#ifndef BOOL
#define BOOL                           int
#endif

#ifndef TRUE
#define TRUE                           1
#endif

#ifndef FALSE
#define FALSE                          0
#endif

#ifndef _MAX_STRING
#define _MAX_STRING                    260
#endif

#ifndef FLT_MAX
#define FLT_MAX                        3.402823466e+38f
#endif

#ifndef FLT_MIN
#define FLT_MIN                        1.175494351e-38f
#endif

#ifndef INVALID_HASHNAME
#define INVALID_HASHNAME               0xffffffff
#endif

#ifndef INVALID_VALUE
#define INVALID_VALUE                  0xffffffff
#endif

#ifndef ASSERT
#define ASSERT                         assert
#endif

#ifndef max
#define max(a,b)                       (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)                       (((a) < (b)) ? (a) : (b))
#endif

#ifndef RGB
#define RGB(r,g,b)                     ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#ifndef RGBA
#define RGBA(r,g,b,a)                  ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))
#endif

#ifndef GET_RED
#define GET_RED(color)                 (BYTE)((color)  &  0xff)
#endif

#ifndef GET_GRN
#define GET_GRN(color)                 (BYTE)(((color) >> 0x08) & 0xff)
#endif

#ifndef GET_BLU
#define GET_BLU(color)                 (BYTE)(((color) >> 0x10) & 0xff)
#endif

#define SWAP(a, b, tmp)                { (tmp) = (a); (a) = (b); (b) = (tmp); }

#define ALIGN_BYTE(a, b)               ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)                 ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)                ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)                ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)                ALIGN_BYTE(a, 4096)


#ifdef _PLATFORM_WINDOWS_
#define CROSS_EXPORT                   __declspec(dllexport)
#else
#define CROSS_EXPORT
#endif
